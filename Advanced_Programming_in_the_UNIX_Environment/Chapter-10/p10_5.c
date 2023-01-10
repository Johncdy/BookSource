#include "apue.h"
#include <stdio.h>
#include <time.h>

#define TRUE    1
#define FALSE   0

#define MAX_TIMES   128
typedef int TIME;
#define VERY_LONG_TIME 2147483647

struct timer {
    int inuse;
    TIME time;
    void (*event)();
} timers[MAX_TIMES];

void timer_interrupt_handler(int);

void timer_init() {
    struct timer *t;

    for (t=timers; t<&timers[MAX_TIMES]; t++)
        t->inuse = FALSE;
    
    if (signal(SIGALRM, timer_interrupt_handler) == SIG_ERR) {
        printf("signal(SIGALRM) error\n");
        exit(0);
    }
}

volatile TIME time_now;

struct timer *timer_next;
TIME time_timer_set;

void timer_update(TIME);

void disable_interrupts() {}
void enable_interrupts() {}

void start_physical_timer(TIME t) {
    printf("start_physical_timer:%d\n", t);
    alarm(t);
}

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
void (*event)();
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
            printf("%d %d\n", t->time, time);
            if (t->time > time) {
                t->time -= time;
                if (t->time < timer_next->time) {
                    timer_next = t;
                }
            } else {
                printf("cccccccccccccccccccc\n");
                t->event();
                t->inuse = 0;
            }
        }
    }
}

void timer_interrupt_handler(int seconds)
{
    printf("timer_interrupt_handler:%d %d\n", time_now, time_timer_set);
    timer_update(time_now - time_timer_set);

    if (timer_next) {
        time_timer_set = time_now;
        start_physical_timer(timer_next->time);
    }
}

void myprint1() { printf("1111111\n"); }
void myprint2() { printf("2222222\n"); }

void update_time()
{
    time_now = time(NULL);

    printf("%d\n", time_now);

    signal(SIGALRM, update_time);

    alarm(1);
}

int main(void)
{
    timer_init();

    pid_t pid;
    if ((pid = fork()) < 0) {
        printf("fork error");
    } else if (pid == 0) {
        if (signal(SIGALRM, update_time) == SIG_ERR)
            printf("signal(SIGALRM) error\n");
        
        alarm(1);
        for(;;)
            pause();
    } else {
        timer_declare(2, myprint1);
        // timer_declare(3, myprint2);
        // timer_declare(4, myprint1);
        // timer_declare(5, myprint2);

        for(;;)
            pause();
    }

    return 0;
}