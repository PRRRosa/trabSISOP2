/*
 * ===========================================================================
 *
 *       Filename:  prod_cons_condvar.c
 *
 *    Description:  Producer/consumer with condition variables
 *
 *        Version:  1.0
 *        Created:  26-04-2015 14:08:50
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
#include "cond.h"

int n_prod;
int n_cons;
int cons_go_on;
int buf_size;
int *buffer;
int produced_limit;


sema_t buf_mtx;
int buf_count;
cond_t full;
cond_t empty;


void dump_blocked_on_cond(cond_t *c) {
    struct list_head *next, *tmp;
    list_for_each_safe(next, tmp, &empty.cond_waiting)
    {
        printf("Thread %p\n", next);
    }

}

struct prod_cons_parms {
    int my_id;
};

void prod(void *p) {
    struct prod_cons_parms *pcp = (struct prod_cons_parms*)p;
    int produced = 0;

    printf("Hello from consumer %d\n", pcp->my_id);

    while(n_prod < produced_limit) {
        int product = -1;

        sema_wait(&buf_mtx);
        while ((buf_count == buf_size) && (n_prod < produced_limit))
            cond_wait(&empty, &buf_mtx);
        //assert(buf_mtx.value <= 0);
        if (n_prod < produced_limit) {
            product = rand();
            buffer[(n_prod++) % buf_size] = product;
            produced++;
            buf_count++;
            printf("\t\t\t\t++ Buf count = %d\n", buf_count);
        }
        thread_yield();

        cond_signal(&full);
        sema_signal(&buf_mtx);

        thread_yield();

        if (product > 0)
            printf("Producer %d produced %d\n", pcp->my_id, product);
    }

    printf("Producer %d is done, pushed %d items into the buffer\n",
            pcp->my_id,
            produced);

    cond_signal(&empty); // Wakes up other producers

    printf("Producer %d quitting\n", pcp->my_id);
    printf("Oh empty>\n");
    dump_blocked_on_cond(&empty);
    printf("On full\n");
    dump_blocked_on_cond(&full);

    int * ret = malloc(sizeof(int));
    *ret = produced;
    thread_exit(ret);
}

void cons(void *p) {
    int product;
    int consumed = 0;
    struct prod_cons_parms *pcp = (struct prod_cons_parms*)p;

    while(cons_go_on) {
        product = -1;
        
        sema_wait(&buf_mtx);

        while ((buf_count == 0) && (n_cons < produced_limit))
            cond_wait(&full, &buf_mtx);

        if (n_cons < produced_limit) {
                product = buffer[(n_cons++) % buf_size];
                consumed++;
                buf_count--;
                printf("\t\t\t\t-- Buf count = %d (cons)\n", buf_count);
        } else {
            cons_go_on = 0;
        }

        thread_yield();
        cond_signal(&empty);
        sema_signal(&buf_mtx);

        thread_yield();
        if (product > 0)
            printf("Consumer %d got %d\n", pcp->my_id, product);
    }

    printf("Calling someone else\n");
    cond_signal(&full); // makes sure other consumers will unblock

    printf("Consumer %d is done, got %d items\n",
            pcp->my_id,
            consumed);

    int * ret = malloc(sizeof(int));
    *ret = consumed;
    thread_exit(ret);
}


int main(int argc, char *argv[]) {
    int prods = 0, conss = 0;
    struct prod_cons_parms *parms;
    thread_t **producers;
    thread_t **consumers;
    int i;

    if (argc < 5) {
        printf("Usage: %s <buf_size> <nproducers> <nconsumers> <producing_limit>",
                argv[0]);
        exit(1);
    }

    buf_size = atoi(argv[1]);
    buf_count = 0;
    prods = atoi(argv[2]);
    conss = atoi(argv[3]);
    produced_limit = atoi(argv[4]);

    producers = malloc(prods*sizeof(thread_t*));
    consumers = malloc(conss*sizeof(thread_t*));
    parms = malloc((prods+conss)*sizeof(struct prod_cons_parms));
    buffer = malloc(buf_size*sizeof(int));

    assert(buffer);
    assert(producers);
    assert(consumers);
    assert(parms);

    threading_init();

    sema_init(&buf_mtx, 1);
    cond_init(&empty);
    cond_init(&full);

    n_prod = n_cons = 0;
    cons_go_on = 1;

    for(i = 0; i < prods; i++) {
        parms[i].my_id = i;
        producers[i] = thread_create(prod, &parms[i]);
    }

    for(i = 0; i < conss; i++) {
        parms[prods+i].my_id = i;
        consumers[i] = thread_create(cons, &parms[prods+i]);
    }

    printf("Created %d producers and %d consumers\n"
           "Now will wait until all of them complete\n",
            prods, conss);

    for(i = 0; i < conss; i++) {
        printf("Calling join for %d\n", i);
        int * consumed = thread_join(consumers[i]);
        printf("Joined, got %p\n", consumed);

        if (!consumed) {
            struct list_head *tmp, *next;
            printf("On empty:\n");
            dump_blocked_on_cond(&empty);
            printf("On full:\n");
            dump_blocked_on_cond(&full);
        }

        printf("Finished consumer %d tells me it consumed %d\n",
                i, *consumed);
    }

    for(i = 0; i < prods; i++) {
        int * produced = thread_join(producers[i]);
        printf("Finished producer %d tells me it produced %d\n",
                i, *produced);
    }

    printf("Calling exit\n");
    threading_exit();
    return 0;
}

