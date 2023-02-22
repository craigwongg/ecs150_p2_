#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

extern queue_t ready_queue;
extern queue_t running_queue;
extern queue_t blocked_queue;


struct semaphore {
	/* TODO Phase 3 */
	size_t count;
	queue_t blocked_threads;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = (sem_t) malloc(sizeof(struct semaphore));
	sem->count = count;
	sem->blocked_threads = queue_create();

	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	queue_destroy(sem->blocked_threads);
	free(sem);
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem->count == 0) {
		queue_enqueue(sem->blocked_threads, uthread_current());
		uthread_block();
	} else {
		sem->count--;
	}
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem->count == 0) {
		struct thread_tcb* temp_tcb;
		queue_dequeue(sem->blocked_threads, (void**)&temp_tcb);
		uthread_unblock(temp_tcb);
	}
	sem->count++;
}

