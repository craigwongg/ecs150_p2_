#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

// Test Create
void test_create(void)
{
	fprintf(stderr, "\n@@@@@ QUEUE_CREATE Test @@@@@\n");
	assert(queue_create() != NULL);
    	fprintf(stderr, "PASSED\n");
}

// Test Enqueue/Dequeue
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "\n@@@@@ QUEUE_SIMPLE Test @@@@@\n");
	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	assert(ptr == &data);
	queue_destroy(q);
    	fprintf(stderr, "PASSED\n");
}

// Test Delete
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;
    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

// Test Iterate
void test_iterator(void)
{
    queue_t q;
    int data[] = {42, 1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 42};
    size_t i;
    fprintf(stderr, "\n@@@@@ QUEUE_ITERATE/QUEUE_DELETE Test @@@@@\n");

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[1] == 2);
    assert(queue_length(q) == 9);
    fprintf(stderr, "PASSED\n");
}

// Test Length
void test_queue_length(void) {
    fprintf(stderr, "\n@@@@@ QUEUE_LENGTH Test @@@@@\n");
    int data = 7;
    queue_t q = queue_create();
    assert(queue_length(q) == 0);
    queue_enqueue(q, &data);
    assert(queue_length(q) == 1);
    fprintf(stderr, "PASSED\n\n");
}

// Test Destroy
void test_queue_destroy(void) {
    fprintf(stderr, "\n@@@@@ QUEUE_Destroy Test @@@@@\n");
    queue_t q1 = queue_create();
    queue_destroy(q1);
    fprintf(stderr, "PASSED\n\n");
}

// Main
int main(void)
{
    test_create();
    test_queue_simple();
    test_iterator();
    test_queue_length();
    test_queue_destroy();
    return 0;
}
