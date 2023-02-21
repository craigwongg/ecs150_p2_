#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

#define RUNNING 0
#define READY 1
#define EXITED 2

/* Service as user scheduler */
static queue_t scheduler;

struct uthread_tcb {
	/* TODO Phase 2 */
	/* This struct will act as queue data and being pushed to scheduler
	 * @ctx:	2d void pointer storing register context
	 * @stack_ptr:		stack pointer of different threads  
	 * @state:			State of Thread RUNNING, READY, EXITED
	 */
	uthread_ctx_t ctx;
	void* stack_ptr;
	int state;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb_handler;
	
	
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb_handler;
	queue_dequeue(scheduler, (void**)&tcb_handler);
	// uthread_ctx_switch(&(tcb_handler->ctx), &(tcb_handler->ctx));
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb = malloc(sizeof(struct uthread_tcb));
	void* sp = uthread_ctx_alloc_stack();

	uthread_ctx_init(&(tcb->ctx), sp, func, arg);
	tcb->stack_ptr = sp;
	tcb->state = READY;

	queue_enqueue(scheduler, tcb);
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	queue_t init_schedule = queue_create();
	scheduler = init_schedule;

	/* Check preemption option */
	if (preempt) {
		
	}

	/* idle thread creation*/
	struct uthread_tcb* tcb_idle = malloc(sizeof(struct uthread_tcb));
	tcb_idle->state == RUNNING;
	queue_enqueue(scheduler, tcb_idle);

	/* New thread: Beginning of thread library */
	uthread_create((uthread_func_t)func, arg);
	while(queue_length(scheduler) != 0) {
		uthread_yield();
	}

	free(scheduler);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}

