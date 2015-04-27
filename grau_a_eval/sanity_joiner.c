/*
 * ===========================================================================
 *
 *       Filename:  sanity_joiner.c
 *
 * Description: Tests any-one-can-join-any-one semantics
 *
 *        Version:  1.0
 *        Created:  26-04-2015 13:39:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigo Virote Kassick (), kassick@gmail.com
 *   Organization:  
 *
 * ===========================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <libthread/thread.h>


struct thread_params {
    thread_t * parent;
    int remaining;
    int me;
};


void thread_fun(void *p) {
    struct thread_params* tp = (struct thread_params *)p;

    printf("Thread %d, remaining %d\n", tp->me, tp->remaining);
    if (tp->remaining) {
        struct thread_params *next = malloc(sizeof(struct thread_params));
        next->parent = current_thread;
        next->remaining = tp->remaining - 1;
        next->me = tp->me + 1;

        thread_t * new = thread_create(thread_fun, next);

        thread_yield();
        if (tp->me % 3) thread_yield();
    }

    printf("Thread %d waiting parent\n", tp->me);
    thread_join(tp->parent);

    printf("Thread %d exiting\n", tp->me);
    thread_exit(NULL);
}

int main(int argc, char *argv[]) {
    thread_t * first;
    struct thread_params *tp = malloc(sizeof(struct thread_params));

    if (argc < 2) {
        printf("Usage: %s <nthreads>\n", argv[0]);
        exit(1);
    }

    threading_init();

    tp->parent = current_thread; //main
    tp->remaining = atoi(argv[1]);
    tp->me = 0;

    first = thread_create(thread_fun, tp);

    printf("Main is calling exit\n");
    thread_exit(NULL);
    printf("All threads done\n");

    threading_exit();
    return 0;
}


