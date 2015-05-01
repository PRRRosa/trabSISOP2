/*
 * =====================================================================================
 *
 *       Filename:  prod_cons.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25-04-2015 15:31:35
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


int n_prod;
int n_cons;
int cons_go_on;
int buf_size;
int *buffer;
int produced_limit;


sema_t buf_mtx;
sema_t empty;
sema_t full;

struct prod_cons_parms {
    int my_id;
};

void prod(void *p) {
    struct prod_cons_parms *pcp = (struct prod_cons_parms*)p;
    int produced = 0;

    while(n_prod < produced_limit) {
        int product = -1;

        sema_wait(&empty);
        thread_yield();
        sema_wait(&buf_mtx);
        thread_yield();

        if (n_prod < produced_limit) {
            product = n_prod;
            buffer[(n_prod++) % buf_size] = product;
            produced++;
        }

        sema_signal(&buf_mtx);
        sema_signal(&full);

        if (product > 0)
            printf("Producer %d produced %d\n", pcp->my_id, product);
    }

    printf("Producer %d is done, pushed %d items into the buffer\n",
            pcp->my_id,
            produced);

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
        
        sema_wait(&full);
        thread_yield();
        sema_wait(&buf_mtx);
        thread_yield();

        if (cons_go_on) {
            if (n_cons == produced_limit)
                cons_go_on = 0;
            else  {
                product = buffer[(n_cons++) % buf_size];
                consumed++;
            }
        }

        sema_signal(&buf_mtx);
        sema_signal(&empty);

        thread_yield();
        if (pcp->my_id % 2)
            thread_yield();

        if (product > 0)
            printf("Consumer %d got %d\n", pcp->my_id, product);
        thread_yield();
    }

    sema_signal(&full); // makes sure other consumers will unblock

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
    sema_init(&empty, buf_size);
    sema_init(&full, 0);

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
        int * consumed = thread_join(consumers[i]);
        printf("Finished consumer %d tells me it consumed %d\n",
                i, *consumed);
    }

    for(i = 0; i < prods; i++) {
        int * produced = thread_join(producers[i]);
        printf("Finished producer %d tells me it produced %d\n",
                i, *produced);
    }

    threading_exit();
    return 0;
}
