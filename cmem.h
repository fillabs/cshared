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
#ifndef cmem_h
#define cmem_h
#include <stdlib.h>
#include <string.h>
#define callocate(N)  malloc(N)
#define callocate0(N) calloc(1,N)
#define cfree(P)      free(P)
#define cnew(T)     (T*)callocate(sizeof(T))
#define cnew0(T)    (T*)callocate0(sizeof(T))
typedef void(cdestructor_fn)(void*);

/** Add X to *P and return old (*P) value
 * @param P pointer to the int variable
 * @param X value to be added to the *P
 * @return old value pointed by P */
#define cfetch_and_add(P,X)  _cfetch_and_add(P,X) 

/** Substitute X from (*P) and return old (*P) value
 * @param P pointer to the int variable
 * @param X value to be substituted from the *P
 * @return old value pointed by P */
#define cfetch_and_sub(P,X)  _cfetch_and_sub(P,X) 

/** Add X to (*P) and return resulting value
 * @param P pointer to the int variable
 * @param X value to be added to the *P
 * @return new value pointed by P */
#define cadd_and_fetch(P,X)  _cadd_and_fetch(P,X) 

/** Substitute X from (*P) and return resulting (*P) value
 * @param P pointer to the int variable
 * @param X value to be substituted from the *P
 * @return new value pointed by P */
#define csub_and_fetch(P,X)  _csub_and_fetch(P,X) 

/** Increment integer variable pointed by P and return old value
 * @param P pointer to the int variable to be incremented
 * @return old value pointed by P */
#define cfetch_and_inc(P)    _cfetch_and_inc(P)   

/** Decrement integer variable pointed by P and return old value
 * @param P pointer to the int variable to be decremented
 * @return old value pointed by P */
#define cfetch_and_dec(P)    _cfetch_and_dec(P)   

/** Increment integer variable pointed by P and return resulting value
 * @param P pointer to the int variable to be incremented
 * @return resulting value pointed by P */
#define cinc_and_fetch(P)    _cinc_and_fetch(P)   

/** Decrement integer variable pointed by P and return new value
 * @param P pointer to the int variable to be decremented
 * @return new value pointed by P */
#define cdec_and_fetch(P)    _cdec_and_fetch(P)

#define ctest_and_swap(P,O,N) _ctest_and_swap(P,O,N)
#define cmem_swap(P,N)        _cmem_swap(P,N)
#define cmem_get(P)           _cmem_get(P)

#include "cbyteswap.h"

#if defined(__GNUC__)
#define _cfetch_and_add(P,X) __sync_fetch_and_add(P, X)
#define _cfetch_and_sub(P,X) __sync_fetch_and_sub(P, X)
#define _cadd_and_fetch(P,X) __sync_add_and_fetch(P, X)
#define _csub_and_fetch(P,X) __sync_sub_and_fetch(P, X)
#define _cfetch_and_inc(P)   _cfetch_and_add(P,1)
#define _cfetch_and_dec(P)   _cfetch_and_sub(P,1)
#define _cinc_and_fetch(P)   _cadd_and_fetch(P,1)
#define _cdec_and_fetch(P)   _csub_and_fetch(P,1)
#define _ctest_and_swap(P,O,N) __sync_val_compare_and_swap (P, O, N)
#define _cmem_swap(P,V)        __atomic_exchange_n (P, V, __ATOMIC_RELEASE)
#define _cmem_get(P)           __atomic_load_n (P, __ATOMIC_CONSUME)

#define __INITIALIZER__(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)

#define CUNUSED __attribute__((unused))
#elif defined (_MSC_VER)
#include <windows.h>
#define _cfetch_and_add(P,X) (InterlockedAddNoFence(P,X)-X)
#define _cfetch_and_sub(P,X) (InterlockedAddNoFence(P,-X)+X)
#define _cadd_and_fetch(P,X) InterlockedAddNoFence(P,X)
#define _csub_and_fetch(P,X) InterlockedAddNoFence(P,-X)
#define _cfetch_and_inc(P)   (InterlockedIncrementNoFence(P)-1)
#define _cfetch_and_dec(P)   (InterlockedDecrementNoFence(P)+1)
#define _cinc_and_fetch(P)   InterlockedIncrementNoFence(P)
#define _cdec_and_fetch(P)   InterlockedDecrementNoFence(P)
#define _ctest_and_swap(P,O,N) InterlockedCompareExchangePointerNoFence(P,N,O)
#define _cmem_swap(P,V)      InterlockedExchangePointer(P,V)
#ifdef _WIN64
#define _cmem_get(P)           (void*)InterlockedAdd64((__int64*)P, 0)
#else
#define _cmem_get(P)           (void*)InterlockedAdd((long*)P, 0)
#endif
#ifndef __cplusplus
#ifndef inline
#define inline _inline
#endif
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*initializer_##f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p "initializer_" #f "_")) \
        static void f(void)
#ifdef _WIN64
#define __INITIALIZER__(f) INITIALIZER2_(f,"")
#else
#define __INITIALIZER__(f) INITIALIZER2_(f,"_")
#endif
#else
#define __INITIALIZER__(f) \
        static void f(void); \
        struct _initializer_##f##_t_ { _initializer_##f##_t_(void) { f(); } }; static _initializer_##f##_t_ _initializer_##f##_; \
        static void f(void)
#endif
#define CUNUSED
#endif

#define struct_from_member(TYPE,POINTER,MEMBER) ((TYPE*)(((const char*)POINTER)-offsetof(TYPE,MEMBER)))

__inline static void * cmemdup(const void * const ptr, size_t size) {
	void * p;
	if (size > 0){
		p = callocate(size);
		if (p && ptr) {
			memcpy(p, ptr, size);
		}
	}
	else{
		p = NULL;
	}
	return p;
}
__inline static void * cmemcpy(void * const dst, size_t max, const void * src, size_t len) {
	size_t size = len > max ? max : len;
	if(size)
		memcpy(dst, src, size);
	return (char*)dst + size;
}

__inline static void * cmemset(void * const dst, int val, size_t size) {
	memset(dst, val, size);
	return (char*)dst + size;
}

__inline static void * cmemzero(void * const dst, size_t len) {
	return cmemset(dst, 0, len);
}

__inline static void * _cretain(void*p, int*prcntr) {
	if (*prcntr != 0){
		cinc_and_fetch(prcntr);
	}
	return p;
}
__inline static void  _crelease(void*p, int*prcntr, void * destr) {
	if (*prcntr != 0){
		if(0 == cdec_and_fetch(prcntr)){
			if (destr) ((cdestructor_fn *)destr)(p);
			else       free(p);
		}
	}
}

#if defined(__GNUC__)
#define cretain_f(S,F) (__typeof__(S)*)_cretain(S, &(S)->F)
#elif defined (_MSC_VER)
#define cretain_f(S,F) _cretain(S, &(S)->F)
#endif
#define cretain(S) cretain_f(S, _rcntr)

#define crelease(S,D) _crelease(S, &(S)->_rcntr, D)

#endif
