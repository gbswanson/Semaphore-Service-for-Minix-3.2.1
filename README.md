Semaphore-Service-for-Minix-3.2.1
=================================
This patch will create a new server that runs on boot allowing for a semaphore service to be integrated into programs.  Four system calls are added allowing for use of the semaphore server.
int sem_init (int x) - creates a semaphore with x number of resources; returns the next available semaphore reference number
int sem_up (int x) - makes one resource available for semaphore x
int sem_down (int x) - decrements resource from semaphore x if available, otherwise program waits until resource is available
int sem_release (int x) - deletes semaphore x if it exists
The application of the semaphore service involving a concurrency problem is demonstrated with main.c, grad.c, and ugrad.c.  
Invoking main will fork six processes, two ugrad and four grad.  Two processes can be in a critical section at a time, however grad and ugrad cannot simultaneously be in a critical section.  
