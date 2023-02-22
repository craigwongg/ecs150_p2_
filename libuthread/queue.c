#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

typedef struct queue {

	/* TODO Phase 1 */
	struct node *head, *tail;
	int size;

}queue;

// Node to keep track of data, as well as prev/next
typedef struct node {

	void* data;
	struct node *next, *prev;
	
}node;

queue_t queue_create(void)
{
	/* TODO Phase 1 */

	// Dynamically allocate space for queue 
	queue* q = (queue*)malloc(sizeof(queue));

	// In case of failure when allocating the new queue
	if (q == NULL) {
		return NULL;
	}

	// Initializing head/tail to NULL 
	q -> head = NULL;
	q -> tail = NULL;
	// Initialize size to 0
	q -> size = 0;
	// Return new queue
	return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */

	// Checking if queue is null or queue is not empty
	if (queue == NULL || queue -> size != 0) {
		return -1;
	}
	
	// Deallocate  
	free(queue);
	return 0;
}
int queue_enqueue(queue_t queue, void *data)
{ 
	/* TODO Phase 1 */

	// Checking to see if queue/data are NULL
	if (queue == NULL || data == NULL) {
		return -1;
	}

	// Allocating a new node for data
	node *n = malloc(sizeof(node));
	if (n == NULL) {
		return -1;
	}

	// Initializing new node
	n -> data = data;
	n -> next = NULL;
	n -> prev = queue -> tail;

	// If queue empty, set head and tail to the new node
	if (queue -> size == 0) {
		queue -> head = n;
		queue -> tail = n;
	}

	// Else, add the new node to end of queue
	else {
		queue -> tail -> next = n;
		n -> prev = queue -> tail;
		queue -> tail = n;
	}

	// Increment size of queue
	queue -> size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */

	// Checking to see if queue/data are NULL, or if queue size is equal to 0
	if (queue == NULL || data == NULL || queue -> size == 0) {
		return -1;
	}
	
	// Get the oldest node from the head of the queue 
	struct node *oldest = queue -> head;
	queue -> head = oldest -> next;
	queue -> size--;
	*data = oldest -> data;

	// Deallocate
	free(oldest);
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */

	// Check if queue or data is NULL
    if (queue == NULL || data == NULL || queue -> size == 0) {
        return -1;
    }
	node *curr_node = queue -> head;
	node *prev_node = NULL;

	while (curr_node != NULL) {

		// Check if current node has data and must be deleted
		if (curr_node -> data == data) {
			// Check if current node is the head node
			if (prev_node == NULL) {
				queue -> head = curr_node -> next;

				// Check if current node is also the tail node
				if (curr_node == queue -> tail) {
					queue -> tail = NULL;
                }

            } 

			else {
				prev_node -> next = curr_node -> next;

				// Check if current node is the tail node
				if (curr_node == queue -> tail) {
					queue -> tail = prev_node;
                }

            }
			
			// Free allocated space and update size of queue
			free(curr_node);
			queue -> size--;
			return 0;
        }
		prev_node = curr_node;
		curr_node = curr_node -> next;
    }

    // Data not found in queue
    return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	
	// Checking if queue/func is equal to NULL
	if (queue == NULL || func == NULL) {
		return -1;
	}
	else {
		// Set current node to the head
		node* currNode = queue -> head;
		node* nextNode;
		
		// Loop through each node in the queue
		while (currNode != NULL) {

			// Get next node in queue
			nextNode = currNode -> next;

			// Call function on the current node data
			func(queue, currNode -> data);

			// If last node is reached return 0
			if (nextNode == NULL) {
				return 0;
			}
			else {
				// Set current node to the next node
				currNode = nextNode;
			}
		}
	}
	
	// If successful, return 0
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */

	// Return -1 when queue is NULL
	if (queue == NULL) {
		return -1;
	}

	// Return queue length
	return queue -> size;	
}
