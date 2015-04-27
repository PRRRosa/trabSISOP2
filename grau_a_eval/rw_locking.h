/*
 * ===========================================================================
 *
 *       Filename:  rw_locking.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26-04-2015 18:15:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigo Virote Kassick (), kassick@gmail.com
 *   Organization:  
 *
 * ===========================================================================
 */

#ifndef __RWLOCKING_H__
#define __RWLOCKING_H__ 1

#include <libthread/thread.h>

typedef struct rw_lock {
    sema_t mutex;
    sema_t wrt;
    int read_count;
} rw_lock_t;

void rw_lock_init(rw_lock_t * rwl) {
    sema_init(&rwl->wrt, 1);
    sema_init(&rwl->mutex, 1);
    rwl->read_count = 0;
}

void lock_reader(rw_lock_t * rwl) {
    sema_wait(&rwl->mutex);
    rwl->read_count++;
    if (rwl->read_count == 1)
        sema_wait(&rwl->wrt);
    sema_signal(&rwl->mutex);
}

void unlock_reader(rw_lock_t * rwl) {
    sema_wait(&rwl->mutex);
    rwl->read_count--;
    if (!rwl->read_count)
        sema_signal(&rwl->wrt);
    sema_signal(&rwl->mutex);
}

void lock_writer(rw_lock_t *rwl) {
    sema_wait(&rwl->wrt);
}

void unlock_writer(rw_lock_t *rwl) {
    sema_signal(&rwl->wrt);
}
#endif
