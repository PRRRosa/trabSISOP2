/*
 * =====================================================================================
 *
 *       Filename:  santy-stack.c
 *
 *    Description:  Tests sanity of stack
 *
 *        Version:  1.0
 *        Created:  26-04-2015 11:54:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
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
    struct thread_fun_params *tp = 
        (struct thread_fun_params*) p;

    int i,j;
    int buf[(tp->end - tp->start)/tp->stride]; // a buffer on the stack!
    // WARNING: May fail if buf is bigger then the stack size used in the
    // library
    


    for (j = 0, i = tp->start;
         i < tp->end;
         i += tp->stride, j++) {
        if (tp->id == 0) {
            thread_yield();
        }
        buf[j] = i;
        thread_yield();
        printf("Thread %d on %i\n", tp->id, buf[j]);
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
