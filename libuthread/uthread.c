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

#define READY 0
#define RUNNING 1
#define EXITED 2

/* Service as user scheduler */
static queue_t ready_queue = NULL;
static queue_t running_queue = NULL;
static queue_t blocked_queue = NULL;

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
	struct uthread_tcb* temp_tcb;
	queue_dequeue(running_queue, (void**)&temp_tcb);
	queue_enqueue(running_queue, temp_tcb);

	return temp_tcb;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb_handler_ready;
	struct uthread_tcb* tcb_handler_running;

	queue_dequeue(ready_queue, (void**)&tcb_handler_ready);
	queue_dequeue(running_queue, (void**)&tcb_handler_running);

	tcb_handler_ready->state = RUNNING;
	tcb_handler_running->state = READY;
	
	queue_enqueue(ready_queue, tcb_handler_running);
	queue_enqueue(running_queue, tcb_handler_ready);
	
	uthread_ctx_switch(&(tcb_handler_running->ctx), &(tcb_handler_ready->ctx));
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb_handler_ready;
	struct uthread_tcb* tcb_handler_exit;

	queue_dequeue(ready_queue, (void**)&tcb_handler_ready);
	queue_dequeue(running_queue, (void**)&tcb_handler_exit);

	tcb_handler_ready->state = RUNNING;

	queue_enqueue(running_queue, tcb_handler_ready);

	uthread_ctx_destroy_stack(tcb_handler_exit->stack_ptr);
	uthread_ctx_switch(&(tcb_handler_exit->ctx), &(tcb_handler_ready->ctx));
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb* tcb = malloc(sizeof(struct uthread_tcb));
	void* sp = uthread_ctx_alloc_stack();

	uthread_ctx_init(&(tcb->ctx), sp, func, arg);
	tcb->stack_ptr = sp;
	tcb->state = READY;

	queue_enqueue(ready_queue, tcb);
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	queue_t init_q_ready = queue_create();
	queue_t init_q_running = queue_create();
	queue_t init_q_block = queue_create();
	ready_queue = init_q_ready;
	running_queue = init_q_running;
	blocked_queue = init_q_block;

	/* Check preemption option */
	if (preempt) {
		
	}

	/* idle thread creation*/
	struct uthread_tcb* tcb_idle = malloc(sizeof(struct uthread_tcb));
	tcb_idle->state == RUNNING;
	queue_enqueue(running_queue, tcb_idle);

	/* New thread: Beginning of thread library */
	uthread_create((uthread_func_t)func, arg);
	while(queue_length(ready_queue) != 0) {
		uthread_yield();
	}

	queue_destroy(ready_queue);
	queue_destroy(running_queue);
	queue_destroy(blocked_queue);
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

