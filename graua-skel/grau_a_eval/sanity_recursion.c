/*
 * ===========================================================================
 *
 *       Filename:  sanity_recursion.c
 *
 *    Description:  Sanity check: recursion works
 *
 *        Version:  1.0
 *        Created:  26-04-2015 13:20:58
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


struct thread_fun_params {
    int id;
    int start, stride, end;
};

void thread_fun(void *p) {
    struct thread_fun_params tp_next, 
                             *tp = (struct thread_fun_params*) p;


    assert(p != &tp_next);
    thread_yield();
    if (tp->start <= tp->end)
    {
        printf("Thread %d on %d\n", tp->id, tp->start);
        tp_next = *tp;
        tp_next.start += tp->stride;
        thread_fun(&tp_next);
        assert(tp->start == tp_next.start - tp_next.stride);
    }

    thread_exit(NULL);
}


int main(int argc, char *argv[]) {
    thread_t ** threads;
    struct thread_fun_params * params;
    int i;
    int nthreads, start, stride, end;

    if (argc < 5) {
        printf("Usage: %s <nthreads> <start> <stride> <end>\n", argv[0]);
        exit(1);
    }

    nthreads = atoi(argv[1]);
    start    = atoi(argv[2]);
    stride   = atoi(argv[3]);
    end      = atoi(argv[4]);

    threads = malloc(nthreads*sizeof(thread_t*));
    params  = malloc(nthreads*sizeof(struct thread_fun_params));

    assert(threads);
    assert(params);

    threading_init();

    for (i = 0; i < nthreads; i++) {
        params[i].id     = i;
        params[i].start  = (i*10)+start;
        params[i].stride = stride;
        params[i].end    = (i*10)+end;

        threads[i] = thread_create(thread_fun, &params[i]);
    }

    printf("%d threads created, waiting for all of them\n", nthreads);

    for (i = 0; i < nthreads; i++) {
        thread_join(threads[i]);
    }

    printf("Finished\n");

    threading_exit();


    return 0;
}
