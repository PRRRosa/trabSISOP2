/*
 * =====================================================================================
 *
 *       Filename:  phil_1.c
 *
 *    Description:  Solução 3: Assimetria esq-dir
 *
 *        Version:  1.0
 *        Created:  22-03-2015 19:31:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <libthread/thread.h>

#define PHIL_MAX_ITER  10000
#define PHIL_CHATTINESS 1000

struct phil_struct {
    sema_t * chopsticks;
};

struct phil_thr_params {
    int phil_i;
    int n_phils;
    struct phil_struct * philosophers;
    struct timeval waiting_time;
};

void phil_thr(void *p_) {
    int iter = PHIL_MAX_ITER;
    struct phil_thr_params *p = (struct phil_thr_params *) p_;

    printf("Philosopher #%d -- ready!\n", p->phil_i);

    while (iter--) {
        struct timeval start, end, res;

        if (iter==(PHIL_MAX_ITER-1) || !(iter % PHIL_CHATTINESS)) {
            printf("Philosopher #%d gonna eat for the %dth time\n", p->phil_i, PHIL_MAX_ITER - iter);
        }

        gettimeofday(&start, NULL);

        if (p->phil_i % 2  == 0) {
            // Even start right
            sema_wait(&p->philosophers->chopsticks[ (p->phil_i+1) % p->n_phils ]);
            thread_yield();
            sema_wait(&p->philosophers->chopsticks[p->phil_i]);
            thread_yield();
        } else {
            // Odd start left
            sema_wait(&p->philosophers->chopsticks[p->phil_i]);
            thread_yield();
            sema_wait(&p->philosophers->chopsticks[ (p->phil_i+1) % p->n_phils ]);
            thread_yield();
        }

        gettimeofday(&end, NULL);

        timersub(&end, &start, &res);

        timeradd(&p->waiting_time, &res, &p->waiting_time);


#ifndef NOSLEEP
        usleep(rand() % 1000);
#endif
        //printf("Philosopher #%d done eating\n", p->phil_i);

        if (p->phil_i % 2 == 0) {
            // Even drops left first
            sema_signal(&p->philosophers->chopsticks[p->phil_i]);
            thread_yield();
            sema_signal(&p->philosophers->chopsticks[ (p->phil_i+1) % p->n_phils ]);
            thread_yield();
        } else {
            // odd drops right first
            sema_signal(&p->philosophers->chopsticks[ (p->phil_i+1) % p->n_phils ]);
            thread_yield();
            sema_signal(&p->philosophers->chopsticks[p->phil_i]);
            thread_yield();
        }

        if (iter==(PHIL_MAX_ITER-1) || !(iter % PHIL_CHATTINESS)) {
            printf("Philosopher #%d gonna think for the %dth time\n", p->phil_i, PHIL_MAX_ITER - iter);
        }

#ifndef NOSLEEP
        usleep(rand() % 100);
#endif
    }

    printf("Philosopher #%d -- done!\n", p->phil_i);

    thread_exit(NULL);

}

int main(int argc, char *argv[])
{
    struct phil_thr_params      *params;
    struct phil_struct          phils;
    thread_t                    **phil_ids;
    int                         i, n_phils;

    n_phils = atoi(argv[1]); // or segfault

    assert (n_phils > 1);

    phils.chopsticks = malloc(n_phils * sizeof(sema_t));
    params = malloc(n_phils * sizeof (struct phil_thr_params));
    phil_ids = malloc(n_phils * sizeof(thread_t*));

    assert(phils.chopsticks);
    assert(params);
    assert(phil_ids);

    threading_init();

    for (i = 0; i < n_phils; i++) {
        sema_init(&phils.chopsticks[i], 1);
    }

    for (i = 0; i < n_phils; i++) {
        params[i].philosophers = &phils;
        params[i].n_phils = n_phils;
        params[i].phil_i = i;
        params[i].waiting_time = (struct timeval){ .tv_sec = 0, .tv_usec = 0 };
    }

    printf("Starting philosophers\n");

    for (i = 0; i < n_phils; i++) {
        phil_ids[i] = thread_create(phil_thr, &params[i]);
    }


    printf("Waiting philosophers\n");

    for (i = 0; i < n_phils; i++) {
        thread_join(phil_ids[i]);
    }


    for (i = 0; i < n_phils; i++) {
        printf("Philosopher #%d waited %ldsecs + %ldusecs\n", 
                i, (long int)params[i].waiting_time.tv_sec,
                (long int)params[i].waiting_time.tv_usec);
    }

    threading_exit();


    return 0;
}

