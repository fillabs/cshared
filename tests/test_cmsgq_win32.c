#include "../cmsgq.h"
#include <stdio.h>
#include <windows.h>

#define R_THREADS 10
#define COUNT 100

static cmsgq_t * queue = NULL;

typedef struct TestMsg{
    cmsg_t _msg;
    char name[32];
    int cnt;
}TestMsg;

static void TestMsg_free(TestMsg* const m) {
    free(m);
}

DWORD threadIdxIndex;
static void m_handler(void * r, TestMsg * m)
{
    intptr_t thr_id = (intptr_t)TlsGetValue(threadIdxIndex);
    if(--m->cnt > 0){
        printf("%03ld: %s enqueue (%d)\n", thr_id, m->name, m->cnt);
        cmsg_enqueue(queue, m, NULL, m_handler);
    }else{
        printf("%03ld: %s free\n", thr_id, m->name);
    }
    fflush(stdout);
}

static void insert(int thr_id, int count) {
    for (int i = 0; i < count; i++) {
        TestMsg* m = cmsg_new(TestMsg);
        m->cnt = 10;
        sprintf_s(m->name, sizeof(m->name), "MSG%03ld_%03d", thr_id, i);
        printf("%03ld: %s enqueue (%d)\n", thr_id, m->name, m->cnt);
        cmsg_enqueue(queue, m, NULL, m_handler);
//        SwitchToThread();
    }
}

DWORD WINAPI r_thread( LPVOID p )
{
    TlsSetValue(threadIdxIndex, p);
    insert((intptr_t)p, COUNT);
    printf("%03ld: Thread exited\n", (intptr_t)p);
    fflush(stdout);
    return 0;
}

int main(int argc, char** argv)
{
    
    HANDLE threads[R_THREADS];
    DWORD  threadIds[R_THREADS];

    cmsgq_t q = {NULL};
    queue = &q;

    insert(0, 10);

    threadIdxIndex = TlsAlloc();
    for (int i = 0; i < R_THREADS; i++) {

        threads[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            r_thread,               // thread function name
            (LPVOID)(i+1),              // argument to thread function 
            0,                      // use default creation flags 
            &threadIds[i]);         // returns the thread identifier 
    }

    while (cmsg_loop(queue)) {
//        SwitchToThread();
    }

    WaitForMultipleObjects(R_THREADS, threads, TRUE, INFINITE);
    for(int i=0; i<R_THREADS; i++) {
        CloseHandle(threads[i]);
    }
    TlsFree(threadIdxIndex);
    return 0;
}
