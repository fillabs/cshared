/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##  Date      : 10.11.2005
##
##  Copyleft (c) 2003 - 2020
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/

#ifndef cmsgq_h
#define cmsgq_h
#include "cring.h"
#include "cmem.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct cmsgq_queue_t cmsgq_queue_t;
    typedef struct cmsgq_t cmsgq_t;

    typedef void (cmsgq_handler_fn)(void * const receiver, cmsgq_t* const msg);
    
    typedef void (cmsgq_destructor_fn)(cmsgq_t* const m);

    struct cmsgq_t {
        cmsgq_t* prev;
        cmsgq_t* next;
        cmsgq_destructor_fn* destructor;
        void* receiver;
        cmsgq_handler_fn* handler;
    };

    struct cmsgq_queue_t {
        cmsgq_t* t;
        cmsgq_t* h;
        int size;
        int collisions;
    };

    void cmsgq_init_queue(cmsgq_queue_t* const q);
    void cmsgq_cleanup_queue(cmsgq_queue_t* const q);

    void _cmsgq_enqueue(cmsgq_queue_t* const q, cmsgq_t* const m, void * const receiver, cmsgq_handler_fn* const h);

#   define cmsgq_enqueue(Q,M,R,H) _cmsgq_enqueue(Q, (cmsgq_t*)(M), R, (cmsgq_handler_fn*) (H))

    int  cmsgq_loop(cmsgq_queue_t* const q);

    cmsgq_t* _cmsgq_init(cmsgq_t* const m, cmsgq_destructor_fn* const d);
#   define cmsgq_init(M,D) _cmsgq_init((cmsgq_t*)(M),(cmsgq_destructor_fn*)(D))
    
    cmsgq_t* _cmsgq_alloc(size_t size, cmsgq_destructor_fn* const d);
#   define cmsgq_alloc(S,D) _cmsgq_alloc(S,(cmsgq_destructor_fn*)(D))
#   define cmsgq_new(T) ((T*)_cmsgq_alloc(sizeof(T),(cmsgq_destructor_fn*)T ## _free))
    
    void cmsgq_free(cmsgq_t* const m);

#ifdef __cplusplus
}
#endif

#endif