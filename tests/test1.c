/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  Exemplo 1 de teste
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


char mensagem[255];

void test_fun(void * a) {
    int i = 0;

    if (!strcmp((char*)a, "oi eu sou um teste"))
        thread_create(test_fun, strdup("i'm another thread"));

    for (i = 0; i < 10; i++) {

        printf("I'm a thread %d, #%d, other is at %s -- %s\n ", 
                thread_get_id(), i, mensagem, (char*)a);
        if ( (i%2) == 0 ) {
            printf("Yielding\n");
            sprintf(mensagem, " -- %d ==", i);
            thread_yield();
        }
    }

    thread_exit(a);
}



int main(int argc, char *argv[])
{
    int i;
    thread_t * t1;
    char * ret;

    if (threading_init()) {
        printf("ooops\n!?");
        exit(1);
    }

    sprintf(mensagem, "NOT INITIALIZED");

    printf("Inited\n");
    t1 = thread_create(test_fun, strdup("oi eu sou um teste"));

    printf("created, yielding\n");

    thread_yield();

    printf("Joining\n");

    ret = (char*) thread_join(t1);

    printf("Joined, got %s\n", ret);

    printf("exiting\n");

    thread_exit(NULL);

    printf ("all finished\n");

    return 0;
}

