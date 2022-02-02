#include "../cmsgq.h"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
DWORD thr_id_key;
#define getThreadId() (TestThread*)TlsGetValue(thr_id_key)
#define setThreadId(ID) TlsSetValue(thr_id_key, ID)
typedef DWORD thread_ret_t;
#define yield() SwitchToThread()
#define snprintf sprintf_s
#else
#include <pthread.h>
#include <stdint.h>
pthread_key_t thr_id_key;
#define getThreadId()   (TestThread*)pthread_getspecific(thr_id_key)
#define setThreadId(ID) pthread_setspecific(thr_id_key, ID)
#define yield() pthread_yield()
typedef void * thread_ret_t;
#endif

#define R_THREADS 20
#define COUNT 100
#define ReCOUNT 10000

typedef struct TestMsg{
    cmsgq_t _msg;
    int cnt;
}TestMsg;


/*
intptr_t getThreadIdentifier() {
    return getThreadId();
}
*/
static void TestMsg_free(TestMsg* const m) {
    free(m);
}

typedef struct TestThread {
    int idx;
    cmsgq_queue_t q;
}TestThread;

static TestThread _threads[R_THREADS];


void print_reindex(cmsgq_queue_t* q)
{
    TestThread* t = struct_from_member(TestThread, q, q);
    printf("%3d: %5d:%-2d : reindex\n", t->idx, t->q.size, t->q.collisions);
}

static void m_handler(void * r, TestMsg * m)
{
    TestThread* t = (TestThread*)r;
    if(--m->cnt > 0){
        int idx;
        idx = rand() % R_THREADS;
        if (idx == t->idx) {
            idx = (idx + 1) % R_THREADS;
        }
        printf("%3d: %5d:%-2d : 0x%p re-enqueue to %d (%d)\n", t->idx, t->q.size, t->q.collisions, m, idx, m->cnt);
        t = &_threads[idx];
        cmsgq_enqueue(&t->q, m, t, m_handler);
    }else{
        printf("%3d: %5d:%-2d :  0x%p free\n", t->idx, t->q.size, t->q.collisions, m);
    }
}

static void insert(TestThread* t, int count) {
    for (int i = 0; i < count; i++) {
        TestMsg* m = cmsgq_new(TestMsg);
        m->cnt = ReCOUNT;
        printf("%3d: %5d:%-2d : 0x%p enqueue (%d)\n", t->idx, t->q.size, t->q.collisions, m, m->cnt);
        cmsgq_enqueue(&t->q, m, t, m_handler);
        yield();
    }
}

static thread_ret_t r_thread(void* p)
{
    TestThread* t = (TestThread*)p;
//    setThreadId(p);
//    insert(t, COUNT);
    while (cmsgq_loop(&t->q)) {
        yield();
    }

    printf("%3d: %5d:%-2d : Thread exited\n", t->idx, t->q.size, t->q.collisions);
    fflush(stdout);
    return (thread_ret_t)0;
}


int main(int argc, char** argv)
{
#ifdef WIN32
    HANDLE threads[R_THREADS];
    DWORD  thrIds[R_THREADS];
#else
    pthread_t threads[R_THREADS];
#endif

    for (int i = 0; i < R_THREADS; i++) {
        _threads[i].idx = i;
        memset(&_threads[i].q, 0, sizeof(_threads[i].q));
        insert(&_threads[i], COUNT);
#ifdef WIN32
        threads[i] = CreateThread(NULL, 0, r_thread, &_threads[i], 0, &thrIds[i]);
#else
        pthread_create(&threads[i], NULL, r_thread, (void*)&_threads[i]);
#endif
    }
 
#ifdef WIN32
    WaitForMultipleObjects(R_THREADS, threads, TRUE, INFINITE);

    for(int i=0; i<R_THREADS; i++) {
        CloseHandle(threads[i]);
    }
 #else
    for(int i=0; i<R_THREADS; i++){
        pthread_join(&threads[i], NULL);
    }
#endif
    return 0;
}
