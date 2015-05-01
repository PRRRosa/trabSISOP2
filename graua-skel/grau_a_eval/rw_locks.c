/*
 * =====================================================================================
 *
 *       Filename:  rw_locks.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25-04-2015 16:37:29
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
#include "rw_locking.h"


struct rwthread_parms {
    int id;
};


rw_lock_t shared_lock;
int shared_value;
int n_writes, n_reads;
sema_t writer_lock;
sema_t reader_lock;

void writer(void * p) {
    int nwrites = 0;
    struct rwthread_parms * rp = 
        (struct rwthread_parms *)p;

    while (1) {
        if ((!(rp->id % 2) && nwrites % 2) ||
            ((rp->id % 2) && !(nwrites % 3) && nwrites))
            thread_yield();
        sema_wait(&writer_lock);
        n_writes--;
        thread_yield();
        sema_signal(&writer_lock);

        if (n_writes < 0)
            break;

        printf("Writer %d wants to write\n", rp->id);
        lock_writer(&shared_lock);
        printf("Writer %d got the lock\n", rp->id);
        shared_value++;
        nwrites++;
        thread_yield();
        printf("Writer %d changed value to %d\n",
                rp->id,
                shared_value);
        unlock_writer(&shared_lock);
    }

    printf("Writer %d finished, wrote %d times\n",
            rp->id,
            nwrites);
    int *ret = malloc(sizeof(int));
    *ret = nwrites;
    thread_exit(ret);

}

void reader(void * p) {
    int nreads = 0;
    struct rwthread_parms * rp = 
        (struct rwthread_parms *)p;

    while (1) {
        if ((!(rp->id % 2) && nreads % 2) ||
            ((rp->id % 2) && !(nreads % 3) && nreads))
            thread_yield();
        sema_wait(&reader_lock);
        n_reads--;
        sema_signal(&reader_lock);

        if (n_reads < 0)
            break;

        printf("Reader %d wants to read\n", rp->id);
        lock_reader(&shared_lock);
        printf("Reader %d got the lock\n", rp->id);
        thread_yield();
        nreads++;
        printf("Reader %d got value %d\n",
                rp->id,
                shared_value);
        unlock_reader(&shared_lock);
        thread_yield();
    }

    printf("Reader %d finished, read %d times\n",
            rp->id,
            nreads);

    int *ret = malloc(sizeof(int));
    *ret = nreads;
    thread_exit(ret);

}
int main(int argc, char *argv[]) {
    int nreaders, nwriters;
    int i;

    thread_t ** readers;
    thread_t ** writers;
    struct rwthread_parms * parms;

    if (argc < 5) {
        printf("Usage: %s <nreaders> <nreads> <nwriters> <nwrites>\n", argv[0]);
        exit(1);
    }

    nreaders = atoi(argv[1]);
    n_reads = atoi(argv[2]);
    nwriters = atoi(argv[3]);
    n_writes = atoi(argv[4]);
    readers = malloc(nreaders*sizeof(thread_t*));
    writers = malloc(nwriters*sizeof(thread_t*));
    parms = malloc((nreaders+nwriters)*sizeof(struct rwthread_parms));

    assert(readers);
    assert(writers);
    assert(parms);

    threading_init();

    rw_lock_init(&shared_lock);
    sema_init(&reader_lock, 1);
    sema_init(&writer_lock, 1);
    shared_value = 0;

    for (i = 0; i < nreaders; i++) {
        parms[i].id = i;
        readers[i] = thread_create(reader, &parms[i]);
    }

    for (i = 0; i < nwriters; i++) {
        parms[nreaders+i].id = i;
        writers[i] = thread_create(writer, &parms[nreaders+i]);
    }

    printf("Created %d readers and %d writers, waiting\n",
            nreaders, nwriters);
    for (i = 0; i < nwriters; i++) {
        int *nwrites = thread_join(writers[i]);
        printf("Writer %d told me it wrote %d times\n",
                i,
                *nwrites);
    }

    for (i = 0; i < nreaders; i++) {
        int *nreads = thread_join(readers[i]);
        printf("Reader %d told me it read %d times\n",
                i,
                *nreads);
    }

    threading_exit();
    return 0;
}
