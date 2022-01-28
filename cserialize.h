/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2003 - 2015
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/

#ifndef cint_h
#define cint_h
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "e4c_lite.h"
#include "cbyteswap.h"

/* serialisation */
FN_THROW(RuntimeException)  int  _cint64_write(const uint64_t n, char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int  _cint32_write(const uint32_t n, char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int  _cint16_write(const uint16_t n, char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int  _cint8_write (const uint8_t  n, char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int  _cintx_write (const uint64_t n, char ** const ptr, const char * const end, int * const error);

#define cint64_write(N,P,S,E) _cint64_write((uint64_t)(N), P, S, E)
#define cint32_write(N,P,S,E) _cint32_write((uint32_t)(N), P, S, E)
#define cint16_write(N,P,S,E) _cint16_write((uint16_t)(N), P, S, E)
#define cint8_write(N,P,S,E)  _cint8_write ((uint8_t)(N), P, S, E)
#define cintx_write(N,P,S,E)  _cintx_write ((uint32_t)(N), P, S, E)
        
FN_THROW(RuntimeException)  uint64_t cint64_read (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  uint32_t cint32_read (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  uint16_t cint16_read (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  uint8_t  cint8_read  (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  uint64_t cintx_read  (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  size_t   cxsize_read (const char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int cintx_bytecount(uint64_t);

FN_THROW(RuntimeException)  int cbuf_write(const void * const p, size_t length, char ** const ptr, const char * const end, int * const error);
FN_THROW(RuntimeException)  int cbuf_read (void * const p, size_t length, const char ** const ptr, const char * const end, int * const error);

typedef struct {
	int    idx;
	void * ptrs[6];
}cbookmark;
int cbookmark_store(cbookmark * bm, char ** const ptr, const char * const end, int * const error);
int cbookmark_apply(cbookmark * bm, char ** const ptr, const char * const end, int * const error);

E4C_DECLARE_EXCEPTION(cexc_readbuf);
E4C_DECLARE_EXCEPTION(RuntimeException);

#ifdef __cplusplus
}
#endif

#endif
