# Userspace Thread Library

This is a simple cooperarative userspace thread library with a FIFO scheduller. The code was produced as a reference implementation of an Operating Systems practical assignment.

Once `thread_init` is called, the `threads` array has two "threads" :
- main (the caller)
- sched : a thread to which we swap context whenever we must schedule

When a thread calls `thread_yield`, the library swaps context with the sched thread, which advances to the next live thread.

Then a thread calls `thread_block`, it marks itself as blocked and swaps context with the sched thread.

Semaphores are implemented using `thread_block` and `thread_wakeup`.
