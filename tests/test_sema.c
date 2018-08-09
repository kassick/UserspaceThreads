/*
 * =====================================================================================
 *
 *       Filename:  test_sema.c
 *
 *    Description:  Semaphore tests
 *
 *        Version:  1.0
 *        Created:  29-03-2015 10:26:22
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
#include <libthread/thread.h>


sema_t *ping_sema;
sema_t *pong_sema;

void ping(void * ignored) {
    int remaining = 10;

    printf("ping started\n");

    while(remaining--) {
        sema_wait(ping_sema);
        printf("Ping %d!\n", 10-remaining);
        sema_signal(pong_sema);
    }

    printf("ping finished\n");

    thread_exit(0);
}

void pong(void * ignored) {
    int remaining = 10;

    printf("pong started\n");

    while(remaining--) {
        sema_wait(pong_sema);
        printf("Pong! %d\n", 10-remaining);
        sema_signal(ping_sema);
    }

    printf("Pong finished\n");

    thread_exit(0);
}


int main(int argc, char *argv[])
{
    thread_t * ping_thread, * pong_thread;

    if (threading_init()) {
        printf("ooops\n!?");
        exit(1);
    }

    ping_sema = sema_create(1);
    pong_sema = sema_create(0);
    if (!ping_sema || !pong_sema)
    {
        printf("ENOMEM while creating the semaphores!?");
        goto exit_fail;
    }


    pong_thread = thread_create(pong, NULL);
    ping_thread = thread_create(ping, NULL);

    printf("created, yielding\n");

    thread_join(ping_thread);
    thread_join(pong_thread);

    printf("exiting\n");

exit_fail:
    thread_exit(NULL);

    printf ("all finished\n");

    return 0;
}
