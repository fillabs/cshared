/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2018
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/

#define COER_ENABLE_EXCEPTIONS
#ifndef casn_oer_h
#define casn_oer_h
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "cserialize.h"
#ifdef COER_ENABLE_EXCEPTIONS
#include "e4c_lite.h"
#else
	#define FN_THROW(X)
#endif

	typedef int (coer_read_fn)  (void * const t, void * const user, const char ** const ptr, const char * const end, int error);
	typedef int (coer_write_fn) (const void * const t, void * const user, char ** const ptr, const char * const end, int error);
	
/* serialisation */
FN_THROW(RuntimeException) int  _coer_write_int        (const int64_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_uint       (const uint64_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_int_range  (const int64_t  n, const int64_t  low, const int64_t  high,
                              char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_length     (const size_t len, char ** const ptr, const char * const end, int error);

FN_THROW(RuntimeException) int  _coer_write_int64      (const int64_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_uint64     (const uint64_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_int32      (const int32_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_uint32     (const uint32_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_int16      (const int16_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_uint16     (const uint16_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_int8       (const int8_t n, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int  _coer_write_uint8      (const uint8_t n, char ** const ptr, const char * const end, int error);

#if 0
#define _coer_write_int64     _cint64_write
#define _coer_write_uint64    _cint64_write
#define _coer_write_int32     _cint32_write
#define _coer_write_uint32    _cint32_write
#define _coer_write_int16     _cint16_write
#define _coer_write_uint16    _cint16_write
#define _coer_write_int8      _cint8_write
#define _coer_write_uint8     _cint8_write

#endif

#define coer_write_uint64(N,P,S,E)  _coer_write_uint64((uint64_t)(N), P, S, E)
#define coer_write_int64(N,P,S,E)   _coer_write_int64((int64_t)(N), P, S, E)
#define coer_write_uint32(N,P,S,E)  _coer_write_uint32((uint32_t)(N), P, S, E)
#define coer_write_int32(N,P,S,E)   _coer_write_int32((int32_t)(N), P, S, E)
#define coer_write_uint16(N,P,S,E)  _coer_write_uint16((uint16_t)(N), P, S, E)
#define coer_write_int16(N,P,S,E)   _coer_write_int16((int16_t)(N), P, S, E)
#define coer_write_uint8(N,P,S,E)   _coer_write_uint8((uint8_t)(N), P, S, E)
#define coer_write_int8(N,P,S,E)    _coer_write_int8((int8_t)(N), P, S, E)
#define coer_write_int(N,P,S,E)     _coer_write_int((int64_t)(N), P, S, E)
#define coer_write_uint(N,P,S,E)    _coer_write_uint((uint64_t)(N), P, S, E)
#define coer_write_int_range(N,L,H,P,S,E) _coer_write_int_range((int64_t)(N), (int64_t)(L), (int64_t)(H), P, S, E)
#define coer_write_length(N,P,S,E)  _coer_write_length((size_t)(N), P, S, E)
#define coer_write_enum(N,P,S,E)    _coer_write_length((size_t)(N), P, S, E)

FN_THROW(RuntimeException) int64_t  coer_read_int(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint64_t coer_read_uint(const char ** const ptr, const char * const end, int error);

#define coer_read_int_range(L,H,P,S,E) _coer_read_int_range((int64_t)(L), (int64_t)(H), P, S, E)
FN_THROW(RuntimeException) int64_t  _coer_read_int_range(const int64_t  low, const int64_t  high,
	const char ** const ptr, const char * const end, int error);

#define coer_read_uint_range(L,H,P,S,E) _coer_read_uint_range((uint64_t)(L), (uint64_t)(H), P, S, E)
FN_THROW(RuntimeException) uint64_t  _coer_read_uint_range(const uint64_t  low, const uint64_t  high,
	const char ** const ptr, const char * const end, int error);

FN_THROW(RuntimeException) size_t coer_read_length(const char ** const ptr, const char * const end, int error);
#define coer_read_enum(P,S,E) (int)coer_read_length(P,S,E)

size_t coer_length_size(size_t len);

FN_THROW(RuntimeException) int64_t   coer_read_int64(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint64_t  coer_read_uint64(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int32_t   coer_read_int32(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint32_t  coer_read_uint32(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int16_t   coer_read_int16(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint16_t  coer_read_uint16(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int8_t    coer_read_int8(const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint8_t   coer_read_uint8(const char ** const ptr, const char * const end, int error);

#define coer_read_uint8_simple(ptr) ((*(const uint8_t*)((*ptr)++)))

#if 0
#define coer_read_int64  cint64_read
#define coer_read_uint64 cint64_read
#define coer_read_int32  cint32_read
#define coer_read_uint32 cint32_read
#define coer_read_int16  cint16_read
#define coer_read_uint16 cint16_read
#define coer_read_int8   cint8_read
#define coer_read_uint8  cint8_read
#endif

#define coer_read_bool(P,S,E)    coer_read_uint8(P,S,E)

FN_THROW(RuntimeException) size_t coer_write_octet_string(const void * const p, size_t length, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) size_t coer_read_octet_string(void * const p, size_t length, size_t max_size, const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) size_t coer_read_octet_string_alloc(void ** const p, size_t length, const char ** const ptr, const char * const end, int error);

FN_THROW(RuntimeException) int coer_write_bit_string(const void * const p, size_t bitlength, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) size_t coer_read_bit_string (void * const p, size_t length, const char ** const ptr, const char * const end, int error);

/*
FN_THROW(RuntimeException) uint64_t coer_read_sequence_header(uint64_t opt_count, const char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int   coer_write_sequence_header(uint64_t opt_count, uint64_t presence_mask, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) int   coer_read_sequence(uint64_t opt_count, void * const p, coer_read_fn read_fn, const char** const ptr, const char* const end, int error);

#define COER_EXTENSIBLE_BIT     (((uint64_t)INT64_MAX)+1)
#define COER_EXTENSIBLE(N)      ((N) | COER_EXTENSIBLE_BIT)
#define COER_IS_EXTENDED(M)     ((int)(((uint64_t)(M))>>63))
#define COER_IS_PRESENT(M,I)    ((M) & (((uint64_t)1)<<(I)))
*/

typedef uint32_t coer_tag_t;
FN_THROW(RuntimeException) int coer_write_tag(coer_tag_t tag, char ** const ptr, const char * const end, int error);
FN_THROW(RuntimeException) coer_tag_t coer_read_tag (const char ** const ptr, const char * const end, int error);
#define COER_TAG_VALUE(T) ((T)&( ( ((coer_tag_t)1) << 31 )-1))
#define COER_TAG(V)  ((V)|(((coer_tag_t)1) << 31 ))
#define coer_read_tag_value_simple(ptr) ((*(const uint8_t*)((*ptr)++))&0x3F)


typedef struct {
	uint8_t prefix;
	uint8_t count;
}coer_sequence_of_header_t;
#define coer_sequence_of_header(ptr) ((*(const coer_sequence_of_header_t**)ptr)++)
FN_THROW(RuntimeException) size_t coer_read_sequence_of(void * const p, coer_read_fn read_fn, const char ** const ptr, const char * const end, int error);

FN_THROW(RuntimeException) char*  _coer_write_sequenceof_count(size_t count, char** const ptr, const char* const end, int error);
FN_THROW(RuntimeException) void   _coer_write_sequenceof_end(char* b, size_t n, const char* end, int error);
FN_THROW(RuntimeException) char*  _coer_write_open_type_length(size_t length, char** const ptr, const char* const end, int error);
FN_THROW(RuntimeException) void   _coer_write_open_type_end(char* b, char** const ptr, const char* const end, int error);

#define coer_read_sequence_of_foreach(N, PTR, END, ERROR) for (size_t N ## _sequence_index= 0, N ## _sequence_count=(size_t)coer_read_uint(PTR, END, ERROR); N ## _sequence_index < N ## _sequence_count; N ## _sequence_index++)
#define coer_read_open_type_do(N, PTR, END, ERROR) for (size_t __ ## N ## __ot_run=1,  N ## _size=coer_read_length(PTR, END, ERROR|FSERR_SIZE); __ ## N ## __ot_run; __ ## N ## __ot_run=0)
#define coer_read_sequence_do(N, PTR, END, ERROR) for (uint8_t __ ## N ## __seq_run=1, N ## _presence_mask=coer_read_uint8(PTR, END, ERROR); __ ## N ## __seq_run; __ ## N ## __seq_run=0)
#define coer_read_choice_do(N, PTR, END, ERROR) for (uint32_t  __ ## N ## __ch_run=1,  N ## _tag=coer_read_tag(PTR, END, ERROR); __ ## N ## __ch_run; __ ## N ## __ch_run=0)

#define coer_write_open_type_do(N,PTR,END,ERROR)  for(char * __ ## N ## _begin = _coer_write_open_type_length(0,PTR,END,ERROR);__ ## N ## _begin; __ ## N ## _begin = NULL)
#define coer_write_open_type_end(N,PTR,END,ERROR)  do { _coer_write_open_type_end(__ ## N ## _begin, PTR, END, ERROR);} while(0)

#define coer_write_sequenceof_do(N,PTR,END,ERROR) for(char * __ ## N ## _begin = _coer_write_sequenceof_count(0, PTR, END, ERROR); __ ## N ## _begin; __ ## N ## _begin = NULL)
#define coer_write_sequenceof_end(N,COUNT,END,ERROR) do { _coer_write_sequenceof_end(__ ## N ## _begin, COUNT,END,ERROR);}while(0)

#define coer_write_sequenceof(N,PTR,END,ERROR) \
	for(struct { size_t count; char * begin; } __ ## N ## _data = {0, _coer_write_sequenceof_count(0, PTR, END, ERROR)}; \
        __ ## N ## _data.begin; \
        _coer_write_sequenceof_end(__ ## N ## _data.begin, __ ## N ## _data.count, END, ERROR), \
		__ ## N ## _data.begin = NULL \
	)
#define set_sequenceof_count(N, COUNT) do { __ ## N ## _data.count = COUNT; }while(0)
#ifdef __cplusplus
}
#endif

#endif
