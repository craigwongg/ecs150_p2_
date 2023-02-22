# Project 2: User-level thread library 

# Summary

The ultimate goal of this project is to better understand the idea of 
threads using a user-level thread library. In order for this to work, 
we must implement a queue API, uthread API, semaphore API, and 
preemption API. After implementation, it simulates how user-based multi-threading functions.

# Implementation
## Queue

Our queue was implemented using a linked list data structure. We chose
this to make our run time optimally efficient and easily modifiable. The
link: https://www.geeksforgeeks.org/queue-linked-list-implementation/ 
guided our implementation of our queue. 

Addind a node struct was essential in storing each node's data as well
the next and previous nodes. We found that all of the functions should 
run in O(1) time except for the delete and iterate functions. Since the 
only way of deleting the data is looping through the queue until the data
was found, this is the most efficient method. As for iterate, the most 
efficient method was also to loop through the queue. 

To ensure that queue.c was fully functional, we utilized a plethora of 
test cases in our queue_tester.c file. 

## Uthread

The Uthread API contain most of the functionallity in multi-threading.
The API itself include it own schedule in order to determine which thread
are the process running. The aforemention Queue API acts as the storage
of the scheduler. In addition, the data type of the queue scheduler are
pointers of uthread_tcb struct. The data structure are the following:
```c
struct uthread_tcb {
	uthread_ctx_t ctx;
	void* stack_ptr;
	int state;
};
```
uthread_tcb struct type stores thread context, stack pointer, and state 
informations in order to support functions that are implemente in this 
API.

This API will initialized an idle thread and a new thread in order to do 
context switches. After it, the new thread will be executed. In spite of
the intialization state, context switches occur when a current thread
yields, exits, or being blocked. However, unblocked does not do context
switches as it only unlock the thread that apear on the block queue. On
the other hand, when another new thread being created, it will be
initialized and append to ready queue, denoting the thread is scheduled
and ready to be run.

The scheduler are definited below:
```c
/* Service as user scheduler */
static queue_t ready_queue = NULL;
static queue_t running_queue = NULL;
static queue_t blocked_queue = NULL;
```
Ready queue represent the thread that are being schedule and ready to be
run; running queue can only have one thread control block store for each
context switch; blocked_queue are records to identify which thread is
being blocked from semaphores. All of the functions in uthread API are
able to manipulate aforementioned schedulers and thread control blocks 
to ensure threads are in correct state.

# Semaphore 

Semaphores, in order words, are generalized locks that prevent starvation
and racing condition. Typically, we will include non-cooperative thread 
cases in this project, blocking threads, ensuring no threads are 
running through critical section of other threads, will be paramount for
the implementation.

The semaphores in this API will also have a counter that represent how
many resources are still available for a thread to take. In addition, it 
include a queue to record down the threads that is being blocked by that 
specific semaphore. In case of a same semaphore blocking multiple threads
without releasing them, blocked_ is declared as queue pointer from queue 
API.
```c
struct semaphore {
	size_t count;
	queue_t blocked_threads;
};
```
sem_down(sem_t sem) and sem_up(sem_t sem) fucntions are consider as one 
of the most important implementations of this API:
```c
int sem_down(sem_t sem);
int sem_down(sem_t sem);
```
Those functions define rather a thread will be blocked and when a thread 
will be released from being blocked. It depends on count as previously
mentioned, "resources" for threads. If count of a semaphore equal to 0, it
means it runs out of resources and need to block the thread (that takes
resources from semaphore). It releases the thread when count increment 
from 0 to 1, indicating that resource is being taken back. The releasing
procedure will dequeue the whole blocked_threads queue as the semaphore 
are up to "giving" out resource again.

# Preempt
