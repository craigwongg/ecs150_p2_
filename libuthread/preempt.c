#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

sigset_t set;
struct sigaction newSig, prevSeg;

void preempt_disable(void)
{
    // sigprocmask(SIG_BLOCK, &set, NULL);
}

void preempt_enable(void)
{
    // sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void preempt_start(bool preempt)
{
    if (preempt) {
        sigemptyset(&set);
        sigaddset(&set, SIGVTALRM);
    }
    else {
        return; 
    }
}

void preempt_stop(void)
{

}

