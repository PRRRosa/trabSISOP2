/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  Esqueleto para biblioteca de thread
 *
 *        Version:  1.0
 *        Created:  29-03-2015 10:06:40
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

#include "thread.h"
#include <list.h>


// Talvez seja útil uma enumeração dos estados de uma thread...
typedef enum {UNITIALIZED = 0, FINISHED, READY, BLOCKED, RUNNING} thread_state_t;
//CRIAR LISTA THREADS(READY, BLOCKED)


int n_threads = 0; // pode ser útil saber quantas threads estão em execução agora
thread_t *current_thread;




struct thread {
    // tudo o que precisa para representar uma thread na biblioteca
    
    //int alguma_coisa_para_nao_dar_erro;
    //CRIAR CONTEXTO
    //getcontext(ucontext_t *ucp);
    ucontext_t ucp;
    //CRIAR ID
    int id;
    //CRIAR ESTADO
    thread_state_t state; 
    //CABECALHO LISTA
    struct listItem* to_list;
    struct ListItem *waitingForJoin;
    void *retval;
};

//http://www1.cs.columbia.edu/~aya/W3101-01/lectures/linked-list.pdf

struct readyList{
	ListItem *head;
};

struct blockedList{
	ListItem *head;
};

// Declaração de current_thread. Retire o comentário e acerte os tipos
    


void threading_sched(void * none) {
    do { 
        printf("You've reached the sched from threads %d, please leave a message\n", thread_get_id());
        // DO SOMETHING
        // Escolhe uma thread, passa o controle para ela
        // SALVAR CONTEXTO THREAD ATUAL

        thread_t * tnext = NULL,
                 * tmp;
        if ((current_thread->to_list != NULL) && (current_thread->state == READY)){
        	tnext = current_thread->to_list->next->t;
        	if (!tnext)
        		if (readyList.head)
        			tnext = readyList.head->t;
        } else {
        	if (readyList.head)
        		tnext = readyList.head->t;
        }

        if (!tnext) {
        	printf("Empty sched\n");
        	exit(1);
        }

        // CHAMA PROXIMA THREAD(SWAP)
        tmp = current_thread;
        current_thread = tnext;
        swapcontext(&tmp->ucp, &current_thread->ucp);

    } while (1);

    // O que fazer quando o escalonador chega aqui?
}

void thread_yield() {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    void *none;
    threading_sched(none);
}

void thread_exit(void * retval) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    current_thread->retval = retval;
    struct listItem* n = current_thread->waitingForJoin;
    while (n != NULL) {
    	struct listItem* _n = n->next;
    	// adiciona a ready
    	n = _n;
    }

    // remover de ready

    current_thread->state = FINISHED;
    //CHAMAR PROXIMA THREAD(YIELD)
    thread_yield();
}

int threading_init() {
    //fprintf(stderr, "NOT IMPLEMENTED\n");

    readyList.head = NULL;
    blockedList.head = NULL;

    thread_t * main_thread = malloc(sizeof(struct thread));
    get_context(&main_thread->ucp);
    main_thread->state = READY;
    main_thread->waitingForJoin = NULL;
    main_thread->id = n_threads;
    n_threads++;

    struct listItem *l = malloc(sizeof( struct listItem));
    l->t = main_thread;
    l->next = NULL;
    main_thread->to_list = l;

    readyList.head = l;
    blockedList.head = NULL;

    return 0;
    //INICIALIZAR THREAD(CONTEXTO, LISTA, ID, ESTADO)
}


void thread_block() {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    //TROCAR ESTADO DA THREAD ATUAL PARA BLOCKED E MANDA PARA LISTA BLOCKED
    current_thread->state = BLOCKED;

    if(current_thread->to_list == readyList->head){
    	readyList.head = current_thread->to_list->next;
    }else{
    	ListItem *prev = readyList.head;
    	while(prev && prev->next != current_thread->to_list){
    		prev = prev->next;
    	}
    	if (prev){
    		prev->next = current_thread->to_list->next;
    	}
    }


    current_thread->to_list->next = blockedList.head;
    blockedList->head = current_thread->to_list;

    thread_yield();
}

void thread_wakeup(thread_t * t) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    //CURRENT PASSA THREAD PARA LISTA READY
	current_thread->state = READY;

    if(current_thread->to_list == blockedList->head){
    	blockedList.head = current_thread->to_list->next;
    }else{
    	ListItem *prev = blockedList.head;
    	while(prev && prev.next != current_thread->to_list){
    		prev = prev->next;
    	}
    	if (prev){
    		prev->next = current_thread->to_list->next;
    	}
    }


    current_thread->to_list->next = readyList.head;
    readyList->head = current_thread->to_list;

    //thread_yield();
    //CURRENT THREAD SET THREAD PARA READY//
    //TALVEZ FAZER YIELD SE ESTIVER NO INICIO DA FILA
}

void * thread_join(thread_t *t) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    if (main_thread->state == FINISHED) {
    	return t->retval;
    } else {
    	// add to joinWaitList
    	// block
    	// return t->retval
    }
    return NULL;
    //RETURN EXIT PARAMETER


}


thread_t * thread_create(thread_fun_t thread_fun, void * param) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    //return NULL;
    //SET ID

    thread_t * new_thread = malloc(sizeof(struct thread));
    new_thread->state = READY;
    new_thread->waitingForJoin = NULL;
    struct listItem *l = malloc(sizeof( struct listItem));
    l->t = new_thread;
    l->next = NULL;
    new_thread->to_list = l;
     l->next = readyList.head;
    readyList.head = l;
    new_thread->id = n_threads;
    n_threads++;

    get_context(&new_thread->ucp);
    new_thread->ucp.uc_link = &current_thread->ucp;
    new_thread->ucp.uc_stack.ss_sp = malloc(4*1024);
    new_thread->ucp.uc_stack.ss_size = 4*1024;
    makecontext(&new_thread->ucp, thread_fun, 1, param);

    //return 0;


    //current_thread->id = n_threads;
    //SET ESTADO
    //current_thread->state = READY;
    //POR NA LISTA
    //readyList->head->t = current_thread;
    //MAKE CONTEXT THREAD_FUN
    //makecontext(thread_fun);
    return new_thread;
}

int thread_get_id() {
    //return 0;

    return current_thread->id;
    // return current_thread
    // return BAZINGA
    // return !?
}

void sema_init(sema_t* s, int n) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    s->count = n;
    s->waitingForSema = NULL;

}

void sema_wait(sema_t* s) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    s->count--;
    if (s->count < 0){
    	current_thread->state = BLOCKED;

    	if(current_thread->to_list == readyList->head){
    		readyList.head = current_thread->to_list->next;
    	}else{
    		ListItem *prev = readyList.head;
    		while(prev && prev->next != current_thread->to_list){
    			prev = prev->next;
    		}
    		if (prev){
    			prev->next = current_thread->to_list->next;
    		} else  {
    			printf("fudeus\n");
    		}
    	}


    	current_thread->to_list->next = s->waitingForSema.head;
    	s->waitingForSema->head = current_thread->to_list;

    	thread_yield();
    }
}


void sema_signal(sema_t* s) {
    //fprintf(stderr, "NOT IMPLEMENTED\n");
    s->count++;
    if (s->waitingForSema->head != NULL) {
    	thread_t * release = s->waitingForSema->t;
		s->waitingForSema = release->to_list->next;
		
		release->to_list->next = readyList.head;
		readyList->head = readyList->to_list;
		release->state = READY;
	}
}
