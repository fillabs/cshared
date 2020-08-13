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

#include "cmsgq.h"
#include <stdio.h>
#include <assert.h>

#define _RECEIVER ((void*)-1)

void cmsgq_init_queue(cmsgq_queue_t* const q)
{
    memset(q, 0, sizeof(*q));
}

void cmsgq_cleanup_queue(cmsgq_queue_t* const q)
{
    cmsgq_t *m;
    do {
        m = q->t;
    } while (m != (cmsgq_t*)ctest_and_swap(&q->t, m, NULL));
    q->h = NULL;
    while (m) {
        cmsgq_t* o = m;
        m = m->prev;
        o->destructor(o);
    }
}

void _cmsgq_enqueue(cmsgq_queue_t* const q, cmsgq_t* const m, void* const receiver, cmsgq_handler_fn* const h)
{
    m->handler = h;
    m->receiver = receiver ? receiver : _RECEIVER;
    cinc_and_fetch(&q->size);
    m->next = NULL;
    for (;;) {
        m->prev = q->t;
        cmsgq_t* o = (cmsgq_t*)ctest_and_swap(&q->t, m->prev, m);
        if (o == m->prev) {
            break;
        }
        cinc_and_fetch(&q->collisions);
    }
}

#ifdef CMSGQ_TEST
void print_reindex(cmsgq_queue_t * q);
#endif

static cmsgq_t* _reindex(cmsgq_queue_t* const q, cmsgq_t* const r)
{
#ifdef CMSGQ_TEST
    size_t count = 0;
    print_reindex(q);
#endif
    cmsgq_t* m = q->t;
    if (m == r) {
        m = NULL;
    }
    else {
        for (; m->prev != r; m = m->prev) {
            m->prev->next = m;
#ifdef CMSGQ_TEST
            if (count > q->size) {
                assert(NULL == "reindex problem");
            }
            count++;
#endif
        }
    }
    return m;
}

#ifdef CMSGQ_MULTITHREAD

intptr_t getThreadIdentifier();

int cmsgq_loop(cmsgq_queue_t* const q)
{
    cmsgq_t* m, * o, * f;
    void* receiver;

    for (;;) {
        o = m = q->h;
        if (m == NULL) {
            m = q->t;
            if (m == NULL) {
                return 0;
            }
            // reindex
            m = _reindex(q, NULL);
        }
        if (o != (cmsgq_t*)ctest_and_swap(&q->h, o, m->next)) {
            cinc_and_fetch(&q->collisions);
            continue;
        }

        f = (cmsgq_t*)ctest_and_swap(&q->t, m, NULL);
        if (f == NULL) {
            // another thread taken the m
            cinc_and_fetch(&q->collisions);
            continue;
        }
        if (m != f) {
            // other items exists in the queue
            o = m->next;
            if (o == NULL) {
                _reindex(q, NULL);
                o = m->next;
            }
            if (m != (cmsgq_t*)ctest_and_swap(&o->prev, m, NULL)) {
                // another thread taken m
                cinc_and_fetch(&q->collisions);
                continue;
            }
        }
        break;
    }

    cdec_and_fetch(&q->size);

    receiver = m->receiver == _RECEIVER ? NULL : m->receiver;
    m->receiver = NULL;
    m->handler(receiver, m);
    if (m->receiver == NULL) {
        // message was not re-sent
        cmsgq_free(m);
    }
    return 1;
}

#else

int cmsgq_loop(cmsgq_queue_t* const q)
{
    cmsgq_t* m;
    void* receiver;

    m = q->h;
    if (m == NULL) {
        m = q->t;
        if (m == NULL) {
            return 0;
        }
        // reindex
        m = _reindex(q, NULL);
    }
    q->h = m->next;
    if (m->next == NULL) {
        cmsgq_t* o = (cmsgq_t*)ctest_and_swap(&q->t, m, NULL);
        if (o != m) {
            o = _reindex(q, m);
            q->h = o;
        }
    }
    cdec_and_fetch(&q->size);

    receiver = m->receiver == _RECEIVER ? NULL : m->receiver;
    m->receiver = NULL;
    m->handler(receiver, m);
    if (m->receiver == NULL) {
        // message was not re-sent
        cmsgq_free(m);
    }
    return 1;
}

#endif

static void _cmsgq_destructor(cmsgq_t* const m){
    // default destructor does nothing!
}

cmsgq_t* _cmsgq_init(cmsgq_t* const m, cmsgq_destructor_fn * const destructor)
{
    memset(m, 0, sizeof(*m));
    m->destructor = destructor ? destructor : _cmsgq_destructor;
    return m;
}

cmsgq_t* _cmsgq_alloc(size_t size, cmsgq_destructor_fn* const destructor)
{
    return _cmsgq_init(callocate(size), destructor);
}

void cmsgq_t_free(cmsgq_t* const m) {
    free(m);
}

void cmsgq_free(cmsgq_t* const m)
{
    m->destructor(m);
}
