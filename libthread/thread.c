/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  Esqueleto para biblioteca de thread
 *
 *        Version:  1.0
 *        Created:  29-03-2015 10:06:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <ucontext.h>

#include "thread.h"
#include <list.h>



#define MAIN_CTX 0
#define SCHED_CTX 255
#define MAX_THREADS 256
#define MAX_STACK_SIZE 4096

typedef enum {UNITIALIZED = 0, FINISHED, READY, BLOCKED, RUNNING} thread_state_t;


struct thread {
    int i;
    void * (*fun)(void *);
    jmp_buf jb;
    ucontext_t t_ctx;
    thread_state_t state;
    void *retval;
    thread_t* join_waitlist[MAX_THREADS];
    int n_join_waitlist;
};


thread_t threads[MAX_THREADS];

int current_thread = MAIN_CTX;
int n_threads = 1;

void threading_sched(void * none) {
    static int i = 0;

    do { 
        //printf("You've reached the sched from threads %d, please leave a message\n",
        //        current_thread);

        if (i != SCHED_CTX && threads[i].state == READY) {
            current_thread = i;
            swapcontext(&threads[SCHED_CTX].t_ctx, &threads[i].t_ctx);
        }

        i = (i + 1) % MAX_THREADS;
    } while (n_threads);

    swapcontext(&threads[SCHED_CTX].t_ctx, &threads[MAIN_CTX].t_ctx);
}

void thread_yield() {
    swapcontext(&threads[current_thread].t_ctx, &threads[SCHED_CTX].t_ctx);
}

void thread_exit(void * ignored) {
    int i;

    if (current_thread != MAIN_CTX) {
        free(threads[current_thread].t_ctx.uc_stack.ss_sp);
    }

    threads[current_thread].state = FINISHED;
    threads[current_thread].retval = ignored;
    n_threads--;

    for (i = 0; i < threads[current_thread].n_join_waitlist; i++)
        thread_wakeup(threads[current_thread].join_waitlist[i]);

    thread_yield();
}

int threading_init() {
    bzero(threads, sizeof(threads));
    char stack[1000];

    if (getcontext(&threads[MAIN_CTX].t_ctx)) return 1;

    threads[SCHED_CTX].t_ctx.uc_link = &threads[MAIN_CTX].t_ctx;
    threads[SCHED_CTX].t_ctx.uc_stack.ss_sp = malloc(MAX_STACK_SIZE);
    threads[SCHED_CTX].t_ctx.uc_stack.ss_size = MAX_STACK_SIZE;
    
    threads[MAIN_CTX].state = threads[SCHED_CTX].state = READY;
    
    getcontext(&threads[SCHED_CTX].t_ctx);

    makecontext(&threads[SCHED_CTX].t_ctx, ( void (*)(void))threading_sched, 1, NULL);

    return 0;
}


void thread_block() {
        threads[current_thread].state = BLOCKED;
        thread_yield();
}

void thread_wakeup(thread_t * t) {
    if (t->state == UNITIALIZED ||
            t->state == FINISHED)
    {
        fprintf(stderr, "Invalid wakeup. It's dead, Jim\n");
    } else {
        t->state = READY;
    }
}
void * thread_join(thread_t *t) {
    int i;

    if (t->state == UNITIALIZED)
        return NULL;

    if (t->state != FINISHED) {
        t->join_waitlist[t->n_join_waitlist++] = &threads[current_thread];
        // blocks until thread finished and wakes up up
        thread_block();
    }

    t->n_join_waitlist--;

    if (!t->n_join_waitlist) {
        t->state = UNITIALIZED;
    }

    return t->retval;
}


thread_t * thread_create(thread_fun_t thread_fun, void * param) {
    int i;
    for (i = 1; i < SCHED_CTX; i++) {
        if (threads[i].state == UNITIALIZED) {
            threads[i].t_ctx.uc_link = &threads[MAIN_CTX].t_ctx;
            threads[i].t_ctx.uc_stack.ss_sp = malloc(MAX_STACK_SIZE);
            threads[i].t_ctx.uc_stack.ss_size = MAX_STACK_SIZE;
            threads[i].retval = NULL;
            threads[i].n_join_waitlist = 0;

            threads[i].state = READY;

            getcontext(&threads[i].t_ctx);
            n_threads++;
            makecontext(&threads[i].t_ctx, ( void (*)(void))thread_fun, 1, param);

            return &(threads[i]);
        }
    }
    return NULL; // no available slots;
}

int thread_get_id() {
    return current_thread;
}
