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
#define BLOCKED 2
#define EXITED 3

/* Service as user scheduler */
static queue_t ready_queue = NULL;
static queue_t running_queue = NULL;
static queue_t blocked_queue = NULL;

struct uthread_tcb {
	/* This struct will act as queue data and being pushed to scheduler
	 * @ctx:	uthread_ctx_t context  pointer storing register context
	 * @stack_ptr:		stack pointer of different threads  
	 * @state:			State of Thread RUNNING, READY, EXITED
	 */
	uthread_ctx_t ctx;
	void* stack_ptr;
	int state;
};

struct uthread_tcb *uthread_current(void)
{
	struct uthread_tcb* temp_tcb;
	queue_dequeue(running_queue, (void**)&temp_tcb);
	queue_enqueue(running_queue, temp_tcb);

	return temp_tcb;
}

void uthread_yield(void)
{
	/* Go back to main threat if nothing on scheduler*/
	if (queue_length(ready_queue) == 0) {
		return;
	}

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
	struct uthread_tcb* tcb_handler_ready;
	struct uthread_tcb* tcb_handler_exit;

	queue_dequeue(ready_queue, (void**)&tcb_handler_ready);
	queue_dequeue(running_queue, (void**)&tcb_handler_exit);

	tcb_handler_ready->state = RUNNING;

	queue_enqueue(running_queue, tcb_handler_ready);

	uthread_ctx_destroy_stack(tcb_handler_exit->stack_ptr);
	free(tcb_handler_exit);
	uthread_ctx_switch(&(tcb_handler_exit->ctx), &(tcb_handler_ready->ctx));
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb* tcb = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	void* sp = uthread_ctx_alloc_stack();

	if(tcb == NULL || sp == NULL) {
		fprintf(stderr, "Error: invalid memory allocation");
		return -1;
	}

	uthread_ctx_init(&(tcb->ctx), sp, func, arg);
	tcb->stack_ptr = sp;
	tcb->state = READY;

	queue_enqueue(ready_queue, tcb);

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
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
	struct uthread_tcb* tcb_idle = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	tcb_idle->state = RUNNING;
	queue_enqueue(running_queue, tcb_idle);

	/* New thread: Beginning of thread library */
	uthread_create((uthread_func_t)func, arg);
	while(queue_length(ready_queue) != 0) {
		if (queue_length(ready_queue) == 0 &&
		    queue_length(blocked_queue) == 0) {
				break;
			}
		uthread_yield();
	}

	queue_destroy(ready_queue);
	queue_destroy(running_queue);
	queue_destroy(blocked_queue);
	return 0;
}

void uthread_block(void)
{
	/* Abnormal case of eventhing is being blocked*/
	if (queue_length(ready_queue) == 0) {
		fprintf(stderr, "Abonamal case of all threads locked!");
		exit(1);
	}

	/* 
	 * @ready_tcb, @running_tcb: TCB pointer
	 * Intitialization to obtain data from queues 
	 */
	struct uthread_tcb* ready_tcb;
	struct uthread_tcb* running_tcb;

	/* 
	 * Taking current running queue to block queue.
	 * To prevent blocked queue to be scheduled for execution
	 */
	queue_dequeue(running_queue, (void**)&running_tcb);
	running_tcb->state = BLOCKED;
	queue_enqueue(blocked_queue, running_tcb);
	
	queue_dequeue(ready_queue, (void**)&ready_tcb);
	ready_tcb->state = RUNNING;
	queue_enqueue(running_queue, ready_tcb);

	uthread_ctx_switch(&(running_tcb->ctx), &(ready_tcb->ctx));
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	uthread->state = READY;
	queue_enqueue(ready_queue, uthread);

	queue_delete(blocked_queue, uthread);
}

