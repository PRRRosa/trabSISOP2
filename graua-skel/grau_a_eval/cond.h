/*
 * =====================================================================================
 *
 *       Filename:  cond.h
 *
 *    Description:  Condition variables for libthread
 *
 *        Version:  1.0
 *        Created:  25-04-2015 13:29:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __COND_H__
#define __COND_H__ 1

#include <libthread/thread.h>
#include "list.h"


struct thread_cond_item {
    struct list_head list;
    thread_t * t;
};

typedef struct cont {
    struct list_head cond_waiting;
} cond_t;

void cond_init(cond_t * cond) {
    cond->cond_waiting.prev = cond->cond_waiting.next = &(cond->cond_waiting);
}

void cond_wait(cond_t * cond, sema_t * mtx) {
    struct thread_cond_item *c_holder = malloc(sizeof(struct thread_cond_item));
    assert(c_holder);

    c_holder->t = current_thread;
    c_holder->list.next = c_holder->list.prev = &(c_holder->list);

    list_add_tail(&c_holder->list, &cond->cond_waiting);

    sema_signal(mtx); // releases the 

    thread_block(); // must remove from ready list

    sema_wait(mtx); // re-acquire mutex
    //assert(mtx->value <= 0);
}

void cond_signal(cond_t * cond) {
    struct thread_cond_item * first;

    if (!list_empty(&cond->cond_waiting)) {
        first = list_first_entry_or_null(&cond->cond_waiting, struct thread_cond_item, list);
        if (!first) {
            fprintf(stderr, "SOMETHING'S ROTTEN IN THE KINGDOM OF DENMARK!\n");
            exit(1);
        }

        list_del(&first->list);
        thread_wakeup(first->t);
        free(first);
    }
}


#endif
