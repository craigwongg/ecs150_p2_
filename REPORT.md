# Project 2: User-level thread library 

# Summary

The ultimate goal of this project is to better understand the idea of 
threads using a user-level thread library. In order for this to work, 
we must implement a queue API, uthread API, semaphore API, and 
preemption API. 

# Queue

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

# Uthread





# Semaphore 





# Preempt
