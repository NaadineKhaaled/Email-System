# Email-System
A Program that implemented the well known Producer-Consumer synchronization problem, as messages come randomly to the system. Counter threads count messages and add them to the counter shared variable, and Monitor thread reads it and places the count in the buffer while Collector thread reads it. Using POSIX and “semaphore.h” libraries
