#include <stdio.h>

#define TRUE    1
#define FALSE   0

#define MAX_TIMES   128
typedef int TIME;
#define VERY_LONG_TIME 2147483647

struct timer {
    int inuse;
    TIME time;
    char *event;
} timers[MAX_TIMES];

void timer_init() {
    struct timer *t;

    for (t=timers; t<&timers[MAX_TIMES]; t++)
        t->inuse = FALSE;
}

volatile TIME time_now;

struct timer *timer_next;
TIME time_timer_set;

void timer_update(TIME);

void disable_interrupts() {}
void enable_interrupts() {}
void start_physical_timer(TIME t) {}

void timer_undeclare(struct timer *t)
{
    disable_interrupts();

    if (!t->inuse) {
        enable_interrupts();
        return;
    }

    t->inuse = FALSE;

    if (t == timer_next) {
        timer_update(time_now - time_timer_set);
        if (timer_next) {
            start_physical_timer(timer_next->time);
            time_timer_set = time_now;
        }
    }

    enable_interrupts();
}

struct timer * timer_declare(time, event)
unsigned int time;
char *event;
{
    struct timer *t;

    disable_interrupts();

    for(t=timers;t<&timers[MAX_TIMES];t++)
        if (!t->inuse) break;
    
    if (t == &timers[MAX_TIMES]) {
        enable_interrupts();
        return 0;
    }

    t->time = time;
    t->event = event;
    if (!timer_next) {
        time_timer_set = time_now;
        start_physical_timer((timer_next = t)->time);
    } else if ((time + time_now) < (timer_next->time + time_timer_set)) {
        timer_update(time_now - time_timer_set);
        time_timer_set = time_now;
        start_physical_timer((timer_next = t)->time);
    }
    t->inuse = TRUE;
    enable_interrupts();
    return t;
}

void timer_update(time)
TIME time;
{
    struct timer timer_last = {
        FALSE,
        VERY_LONG_TIME,
        NULL
    };

    struct timer *t;
    timer_next = &timer_last;

    for (t=timers;t<&timers[MAX_TIMES];t++) {
        if (t->inuse) {
            if (t->time > time) {
                t->time -= time;
                if (t->time < timer_next->time) {
                    timer_next = t;
                }
            } else {
                *t->event = TRUE;
                t->inuse = 0;
            }
        }
    }
}

void timer_interrupt_handler()
{
    timer_update(time_now - time_timer_set);

    if (timer_next) {
        time_timer_set = time_now;
        start_physical_timer(timer_next->time);
    }
}