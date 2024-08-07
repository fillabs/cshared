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
FN_THROW(RuntimeException) uint32_t coer_read_bit_mask(size_t bit_count, const char ** ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint32_t coer_read_bit_mask_revers(size_t length, const char ** ptr, const char * const end, int error);
FN_THROW(RuntimeException) uint32_t coer_read_bit_mask_length(size_t * length, const char ** ptr, const char * const end, int error);

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

/**
 * @brief Read SEQUENCE-OF. Call callback function for each element.
 * @param p - user pointer to be sent to the callback
 * @param read_fn - user callback function to be called for each element
 * @param ptr - const char** pointer to the buffer
 * @param end - const char* pointer to the buffer end
 * @param error - integer error to thrown in case of error
 * @return count of bytes read from buffer
 */
FN_THROW(RuntimeException) size_t coer_read_sequence_of(void * const p, coer_read_fn read_fn, const char ** const ptr, const char * const end, int error);

FN_THROW(RuntimeException) char*  _coer_write_sequenceof_count(size_t count, char** const ptr, const char* const end, int error);
FN_THROW(RuntimeException) void   _coer_write_sequenceof_end(char* b, size_t n, char** const ptr, const char* end, int error);
FN_THROW(RuntimeException) char*  _coer_write_open_type_length(size_t length, char** const ptr, const char* const end, int error);
FN_THROW(RuntimeException) void   _coer_write_open_type_end(char* b, char** const ptr, const char* const end, int error);

typedef struct coer_ot_t {
    size_t    size;
    uint8_t * buf;
	const char * end;
}coer_opentype_t;
#define coer_read_ot_do(N, PTR, END, ERROR) for (coer_opentype_t N = {coer_read_length(PTR, END, ERROR),(uint8_t*)*(PTR), (*PTR)+N.size}; N.size != (size_t)-1; (*PTR)=(const char*)(N.buf+N.size), N.size = (size_t)-1)

#define coer_read_open_type_do(N, PTR, END, ERROR)         for (size_t __ ## N ## __ot_run=1,  N ## _size=coer_read_length(PTR, END, ERROR); __ ## N ## __ot_run; __ ## N ## __ot_run=0)

/**
 * @brief read SEQUENCE from OER buffer
 * @param N       any          - any unique identifier for the SEQUENCE
 * @param OPT_CNT uint32_t     - count of optional and defaul elements.
 *                               If sequence is extendable, this value should be incremented by 1.
 * @param PTR     const char** - pointer to advanceble pointer to OER buffer. 
 * @param END     const char*  - pointer to end of OER buffer. 
 * @param ERROR   int          - integer value to be thrown if error occured
 *  
 * For sequence:
 * SEQUENCE {
 *   n1 INTEGER, -- mandatory element
 *   n2 INTEGER OPTIONAL,  -- optional element
 *   n3 INTEGER DEFAULT 0, -- optional default element
 *   ...,        -- extension marker
 *   n4 INTEGER, -- 1st extended element
 *   n5 INTEGER  -- 2nd extended element
 * }
 * The following code snippet can be used:
 * coer_read_sequence_do (SOME_SEQ, 3, &ptr, end, error){ // two optional elements and 1 for extension
 *     n1 = coer_read_int(&ptr, end, error);
 *     if(SOME_SEQ_presence_mask & 0x02) { // has n2, extension presense bit is in first position
 *       n2 = coer_read_int(&ptr, end, error);
 *     }
 *     if(SOME_SEQ_presence_mask & 0x03) { // has n3
 *       n3 = coer_read_int(&ptr, end, error);
 *     }else{                              // or set default value
 *       n3 = 0; 
 *     }
 *     coer_read_sequence_extensions_do(SOME_SEQ, &ptr, end, error){ // check for extension
 *       if(SOME_SEQ_extensions_mask & 0x01) { // has n4
 *         n4 = coer_read_int(&ptr, end, error);
 *       }
 *       SOME_SEQ_extensions_mask &= ~0x01;                            // unset presence for n4
 *       coer_skip_sequence_extensions(SOME_SEQ, &ptr, end, error); // and skip all other extensions: n5, etc.
 * 	   }
 * }
 */
#define coer_read_sequence_do(N, OPT_CNT, PTR, END, ERROR)  \
    for ( uint32_t __ ## N ## __seq_run=1, CUNUSED N ## _opt_cnt=OPT_CNT, N ## _presence_mask = (N ## _opt_cnt ? coer_read_bit_mask_revers(N ## _opt_cnt, PTR, END, ERROR) : 0); __ ## N ## __seq_run; __ ## N ## __seq_run=0)
#define coer_read_sequence_extensions_do(N, PTR, END, ERROR) \
    for( uint32_t N##_extensions_mask = (N##_presence_mask & 1)?coer_read_bit_mask_revers(-1, PTR, END, ERROR):0;N##_extensions_mask;N##_extensions_mask=0)
#define coer_skip_sequence_extensions(N, PTR, END, ERROR) \
	for(;N##_extensions_mask;N##_extensions_mask >>= 1) { \
		if (1 & N##_extensions_mask){ \
			size_t _l = coer_read_length(PTR, END, ERROR); \
			if(END < (_l + (*(PTR)))) { throw(RuntimeException, ERROR, NULL); } \
			*(PTR) += _l; \
		} \
	}

#define coer_read_choice_do(N, PTR, END, ERROR) for (uint32_t  __ ## N ## __ch_run=1,  N ## _tag=coer_read_tag(PTR, END, ERROR); __ ## N ## __ch_run; __ ## N ## __ch_run=0)

/**
 * @brief Write OPENTYPE and the OPENTYPE content
 * @param NAME any         - Name of the sequence. Any unique identifier.
 * @param LEN  size_t      - size of the data or 0 if unknown.
 * @param PTR  char**      - pointer to the buffer.
 * @param END  const char* - pointer to the buffer end
 * @param ERROR int       - integer error to be thrown in case of error
 * 
 * Usage:
 * coer_write_open_type(any_unique_identifier,0, &ptr,end,error){
 *      // write internal data type
 * }
 * The body in parenthesis {} will be executed once.
 */
#define coer_write_open_type(NAME,LEN,PTR,END,ERROR) \
    for(char * __ ## NAME ## _begin = _coer_write_open_type_length(LEN, PTR, END, ERROR); \
	     __ ## NAME ## _begin; \
		 _coer_write_open_type_end(__ ## NAME ## _begin, PTR, END, ERROR), __ ## NAME ## _begin = NULL)

/**
 * @brief Read SEQUENCE OF Type from the stream
 * @param N   any         - Name of the sequence. Any unique identifier.
 * @param PTR char**      - pointer to the buffer.
 * @param END const char* - pointer to the buffer end
 * @param ERROR int       - integer error to be thrown in case of error
 * 
 * Usage:
 * coer_read_sequence_of_do(any_seq,&ptr,end,error){
 *     // read one element
 * }
 * The body in parenthesis {} will be executed for each element.
 * Following values defined within the body
 *   size_t any_seq_sequence_count - count of elements
 *   size_t any_seq_sequence_index - index of the current element
 * Application MUST read data from the stream and advice ptr correctly inside the body
 */
#define coer_read_sequence_of_do(N, PTR, END, ERROR) for (size_t N ## _sequence_index= 0, N ## _sequence_count=(size_t)coer_read_uint(PTR, END, ERROR); N ## _sequence_index < N ## _sequence_count; N ## _sequence_index++)

/**
 * @brief Write SEQUENCE OF Type to the stream
 * @param N   any         - Name of the sequence. Any unique identifier.
 * @param PTR char**      - pointer to the buffer.
 * @param END const char* - pointer to the buffer end
 * @param ERROR int       - integer error to be thrown in case of error
 * @param COUNT size_t    - count of elements to be writen.
 * 
 * Usage:
 * coer_write_sequenceof_do(any_seq,&ptr,end,error){
 *	    size_t i;
 *	    for(i=0; has_elements(i); i++){ // for each element
 *	        // write i-th element
 *      }
 *      coer_write_sequenceof_end(any_seq, i, &ptr, end, error)
 * }
 * The body in parenthesis {} will be executed once.
 * coer_write_sequenceof_end shall be called at the end of the body.
 */
#define coer_write_sequenceof_do(N,PTR,END,ERROR) for(char * __ ## N ## _begin = _coer_write_sequenceof_count(0, PTR, END, ERROR); __ ## N ## _begin; __ ## N ## _begin = NULL)
#define coer_write_sequenceof_end(N,COUNT,PTR,END,ERROR) do { _coer_write_sequenceof_end(__ ## N ## _begin, COUNT,PTR,END,ERROR);}while(0)

/*
#define coer_write_sequenceof(N,PTR,END,ERROR) \
	for(struct { size_t count; char * begin; } __ ## N ## _data = {0, _coer_write_sequenceof_count(0, PTR, END, ERROR)}; \
        __ ## N ## _data.begin; \
        _coer_write_sequenceof_end(__ ## N ## _data.begin, __ ## N ## _data.count, END, ERROR), \
		__ ## N ## _data.begin = NULL \
	)
#define set_sequenceof_count(N, COUNT) do { __ ## N ## _data.count = COUNT; }while(0)

char * _coer_write_sequenceof_begin (char ** ptr, const char * end, int error);
char * _coer_write_sequenceof_update(char * plen, char ** ptr, const char * end, int error);
*/

#ifdef __cplusplus
}
#endif

#endif
