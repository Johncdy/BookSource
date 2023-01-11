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
}

volatile TIME time_now;

struct timer *timer_next;
TIME time_timer_set;

void timer_update(TIME);

void disable_interrupts() {}
void enable_interrupts() {}

void start_physical_timer(TIME t) {
    if (signal(SIGALRM, timer_interrupt_handler) == SIG_ERR) {
        printf("signal(SIGALRM) error\n");
        exit(0);
    }
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

    time_now = time(NULL);

    if (t == timer_next) {
        timer_update(time_now - time_timer_set);
        if (timer_next) {
            start_physical_timer(timer_next->time);
            time_timer_set = time_now;
        }
    }

    enable_interrupts();
}

struct timer * timer_declare(dtime, event)
unsigned int dtime;
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

    t->time = dtime;
    t->event = event;

    time_now = time(NULL);

    if (!timer_next) {
        time_timer_set = time_now;
        start_physical_timer((timer_next = t)->time);
    } else if ((dtime + time_now) < (timer_next->time + time_timer_set)) {
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
                t->event();
                t->inuse = 0;
            }
        }
    }

    if (timer_next == &timer_last)
        timer_next = NULL;
}

void timer_interrupt_handler(int seconds)
{
    time_now = time(NULL);
    timer_update(time_now - time_timer_set);

    if (timer_next) {
        time_timer_set = time_now;
        start_physical_timer(timer_next->time);
    }
}

void myprint1() { printf("1111111\n"); }
void myprint2() { printf("2222222\n"); }

volatile sig_atomic_t quitflag;

void sig_int(int signo)
{
    if (signo == SIGINT) {
        printf("\ninterrupt\n");
    }
    else if (signo == SIGQUIT) {
        printf("\nquit\n");
        quitflag = 1;
    }
}

int main(void)
{
    timer_init();

    if (signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        printf("signal(SIGQUIT) error");
        
    alarm(1);

    timer_declare(2, myprint1);
    timer_declare(3, myprint2);
    timer_declare(4, myprint1);
    timer_declare(8, myprint2);

    while (quitflag == 0)
    {
        pause();
    }
    
    exit(0);
}