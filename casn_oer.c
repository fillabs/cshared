/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2018
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/

#include "casn_oer.h"
#include "cserialize.h"
#include "cstr.h"
#include "e4c_lite.h"
#include <errno.h>
#include <string.h>

#ifdef COER_ENABLE_EXCEPTIONS
#define THROW_ERROR(E) throw(RuntimeException, (error | E), NULL)
#else
#define THROW_ERROR(E)
#endif

int64_t   coer_read_int64(const char ** const ptr, const char * const end, int error)
{
	return cint64_read(ptr, end, &error);
}
uint64_t  coer_read_uint64(const char ** const ptr, const char * const end, int error)
{
	return (uint64_t)cint64_read(ptr, end, &error);
}
int32_t   coer_read_int32(const char ** const ptr, const char * const end, int error)
{
	return cint32_read(ptr, end, &error);
}
uint32_t  coer_read_uint32(const char ** const ptr, const char * const end, int error)
{
	return (uint32_t)cint32_read(ptr, end, &error);
}
int16_t   coer_read_int16(const char ** const ptr, const char * const end, int error)
{
	return cint16_read(ptr, end, &error);
}
uint16_t  coer_read_uint16(const char ** const ptr, const char * const end, int error)
{
	return (uint16_t)cint16_read(ptr, end, &error);
}
int8_t    coer_read_int8(const char ** const ptr, const char * const end, int error)
{
	return cint8_read(ptr, end, &error);
}
uint8_t   coer_read_uint8(const char ** const ptr, const char * const end, int error)
{
	return (uint8_t)cint8_read(ptr, end, &error);
}
int  _coer_write_int64(const int64_t n, char ** const ptr, const char * const end, int error)
{
	return _cint64_write(n, ptr, end, &error);
}
int  _coer_write_uint64(const uint64_t n, char ** const ptr, const char * const end, int error)
{
	return _cint64_write(n, ptr, end, &error);
}
int  _coer_write_int32(const int32_t n, char ** const ptr, const char * const end, int error)
{
	return _cint32_write(n, ptr, end, &error);
}
int  _coer_write_uint32(const uint32_t n, char ** const ptr, const char * const end, int error)
{
	return _cint32_write(n, ptr, end, &error);
}
int  _coer_write_int16(const int16_t n, char ** const ptr, const char * const end, int error)
{
	return _cint16_write(n, ptr, end, &error);
}
int  _coer_write_uint16(const uint16_t n, char ** const ptr, const char * const end, int error)
{
	return _cint16_write(n, ptr, end, &error);
}
int  _coer_write_int8(const int8_t n, char ** const ptr, const char * const end, int error)
{
	return _cint8_write(n, ptr, end, &error);
}
int  _coer_write_uint8(const uint8_t n, char ** const ptr, const char * const end, int error)
{
	return _cint8_write(n, ptr, end, &error);
}

uint64_t  _coer_read_uint_range(const uint64_t  low, const uint64_t  high,
                                const char ** const ptr, const char * const end, int error)
{
	if (high == -1) {
		return coer_read_uint(ptr, end, error);
	}
	if(high < low) {
		THROW_ERROR(EINVAL);
		return (uint64_t)-1;
	}
	if(high < 0x100){
		return (uint64_t)coer_read_uint8(ptr,end,error);
	}
	if(high < 0x10000){
		return (uint64_t)coer_read_uint16(ptr,end,error);
	}
	if(high < 0x100000000){
		return (uint64_t)coer_read_uint32(ptr,end,error);
	}
	return coer_read_uint64(ptr, end, error);
}

int64_t  _coer_read_int_range(const int64_t  low, const int64_t high,
                               const char ** const ptr, const char * const end, int error)
{
	if(low >= 0) {
		return (int64_t)_coer_read_uint_range((const uint64_t)low, (const uint64_t)high, ptr, end, error);
	}
	if (low == -1 || high == -1) {
		return (int64_t)coer_read_int(ptr, end, error);
	}
	if(low >= -128 && high <= 127) {
		return (int64_t)(int8_t)coer_read_uint8(ptr,end,error);
	}
	if(low >= -32768 && high <= 32767) {
		return (int64_t)(int16_t)coer_read_uint16(ptr,end,error);
	}
	if(low >= ((int64_t)0)-2147483648 && high <= 2147483647) {
		return (int64_t)(int32_t)coer_read_uint32(ptr,end,error);
	}
	return (int64_t)coer_read_uint64(ptr,end,error);
}

int  _coer_write_uint_range (const int64_t  n, const int64_t  low, const int64_t  high,
                                    char ** const ptr, const char * const end, int error)
{
	if (high == -1) {
		return coer_write_uint(n, ptr, end, error);
	}
	if(high < low) {
		THROW_ERROR(EINVAL);
		return -1;
	}
	if(high < 0x100){
		return coer_write_uint8(n, ptr,end,error);
	}
	if(high < 0x10000){
		return coer_write_uint16(n, ptr,end,error);
	}
	if(high < 0x100000000){
		return coer_write_uint32(n, ptr,end,error);
	}
	return coer_write_uint64(n,ptr,end,error);
}

int  _coer_write_int_range  (const int64_t  n, const int64_t  low, const int64_t  high,
                              char ** const ptr, const char * const end, int error)
{
	if (low >= 0) {
		return _coer_write_uint_range((const uint64_t)n, (const uint64_t)low, (const uint64_t)high, ptr, end, error);
	}
	if(low == -1 || high == -1) {
		return coer_write_int(n, ptr, end, error);
	}
	if(low >= -128 && high <= 127) {
		return coer_write_uint8(n, ptr,end,error);
	}
	if(low >= -32768 && high <= 32767) {
		return coer_write_uint16(n, ptr,end,error);
	}
	if (low >= ((int64_t)0) - 2147483648 && high <= 2147483647) {
		return coer_write_uint32(n, ptr, end, error);
	}
	return coer_write_uint64(n, ptr,end,error);	
}

int  _coer_write_int(const int64_t n, char ** const ptr, const char * const end, int error)
{
	return _coer_write_uint((uint64_t)n, ptr, end, error);
}

int  _coer_write_uint(const uint64_t n, char ** const ptr, const char * const end, int error)
{
	size_t len = cintx_bytecount(n);
	coer_write_length(len, ptr, end, error);
	const unsigned char * s = (const unsigned char *)&n;
	unsigned char * d = *(unsigned char **)ptr;
	*ptr = (char*) d + len;
#if	(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	len--;
	for (size_t i = 0; i <= len; i++) {
		d[i] = s[len - i];
	}
#else
	for (size_t i = 0; i < len; i++) {
		d[i] = s[8 - len - i];
	}
#endif
	return 0;
}

int64_t coer_read_int(const char ** const ptr, const char * const end, int error)
{
	return (int64_t)coer_read_uint(ptr, end, error);
}

static uint64_t _coer_read_int_with_length(const uint64_t nlen, const char ** const ptr, const char * const end, int error);

uint64_t coer_read_uint(const char ** const ptr, const char * const end, int error)
{
	size_t length = coer_read_length(ptr, end, error);
	return _coer_read_int_with_length(length, ptr, end, error);
}

static uint64_t _coer_read_int_with_length (const uint64_t nlen, const char ** const ptr, const char * const end, int error)
{
	uint64_t value;
	register const uint8_t *p, *e;
	if(nlen > 8) {
		THROW_ERROR(EINVAL);
		return (unsigned)-1;
	}
	p = (const  uint8_t *)*ptr;
	e = p + nlen;
	if (e > (const uint8_t *)end) {
		THROW_ERROR(EFAULT);
		return (unsigned)-1;
	}

	value=0;
	for (; p < e; p++){
		value  = (value<<8) | *p;
	}
	*ptr = (char*)p;
	return value;
}

size_t coer_length_size(size_t len)
{
	size_t l = 1;
	if (len >= 128)
		l += cintx_bytecount(len);
	return l;
}

size_t coer_read_length(const char ** const ptr, const char * const end, int error)
{
	uint8_t n;
	size_t ret;
	if (*ptr >= end) {
		THROW_ERROR(EFAULT);
		return (uint64_t)-1;
	}
	n = *(const uint8_t*)((*ptr)++);
	
	if(0 == (n & 0x80)) {
		ret = n;
	}
	else {
		ret = (size_t)_coer_read_int_with_length(n & 0x7F, ptr, end, error);
		if ((*ptr) + ret > end) {
			THROW_ERROR(EFAULT);
			return (uint64_t)-1;
	}
	}
	return ret;
}

int  _coer_write_length(const size_t len, char ** const ptr, const char * const end, int error)
{
	register uint8_t *p, *e;
	uint8_t l;
	uint64_t value = len;

	if (value < 128){
		return coer_write_int8(value, ptr, end, error);
	}

	l = cintx_bytecount(value);

	p = (uint8_t *)*ptr;
	e = p + l;
	if (e >= (const uint8_t*)end){
		THROW_ERROR(ENOSPC);
		return -1;
	}
	*p = l + 0x80;

	while (p < e){
		*(e--) = (uint8_t)(value & 0xFF);
		value >>= 8;
	}
	*ptr = (char*) p + l + 1;
	return 0;
}
/*
uint64_t coer_read_sequence_header(uint64_t opt_count, const char ** const ptr, const char * const end, int error)
{
	const uint8_t *p, *e;
	uint64_t mask = 0;
	int extendable = COER_IS_EXTENDED(opt_count);
	opt_count = opt_count & INT64_MAX;
	if (opt_count > 32) {
		THROW_ERROR(EINVAL);
		return (uint64_t)-1;
	}
	p = *(const uint8_t **)ptr;
	e = p + (opt_count + extendable + 7) / 8;
	if (e > (const uint8_t *)end){
		THROW_ERROR(ENOSPC);
		return (uint64_t)-1;
	}
	for(; p < e; p++) {
		mask = (mask << 8) | *p;
	}
	*ptr = (char*)p;
	if (extendable && (mask & (((uint64_t)1) << opt_count))){
		uint64_t e_mask = 0;
		size_t len;
		mask &= COER_EXTENSIBLE_BIT | (mask & ~((((uint64_t)1) << opt_count)));
		// read extesible mask;
		len = coer_read_bit_string(&e_mask, sizeof(e_mask), ptr, end, error);
		if (len < 0){
			return (uint64_t)len;
		}
		mask |= (e_mask << 32) | COER_EXTENSIBLE_BIT;
	}
	return mask;
}

FN_THROW(RuntimeException) int coer_read_sequence(uint64_t opt_count, void* const p, coer_read_fn read_fn, const char** const ptr, const char* const end, int error)
{
	uint64_t mask = coer_read_sequence_header(opt_count, ptr, end, error);
	return read_fn((void*)&mask, p, ptr, end, error);
}

int coer_write_sequence_header(uint64_t opt_count, uint64_t presence_mask, char ** const ptr, const char * const end, int error)
{
	THROW_ERROR(EACCES);
	return -1;

}
*/
size_t coer_read_octet_string_alloc(void ** const p, size_t length, const char ** const ptr, const char * const end, int error)
{
	if (length == (size_t)-1) {
		length = coer_read_length(ptr, end, error);
	}
	if (length > 0) {
		char * s = cstralloc(length);
		cbuf_read(s, length, ptr, end, &error);
		*(char**)p = s;
	}
	return length;
}

size_t coer_read_octet_string(void * const p, size_t length, size_t max_size, const char ** const ptr, const char * const end, int error)
{
	size_t l;
	if (length == (size_t)-1) {
		length = coer_read_length(ptr, end, error);
	}
	l = length > max_size ? max_size : length;
	cbuf_read(p, l, ptr, end, &error);
	if (l < length) {
		cbuf_read(NULL, length - l, ptr, end, &error);
	}
	return l;
}

size_t coer_write_octet_string(const void * const p, size_t length, char ** const ptr, const char * const end, int error)
{
	const char * s = *ptr;
	coer_write_length(length, ptr, end, error);
	cbuf_write(p, length, ptr, end, &error);
	return *ptr - s;
}

size_t coer_read_bit_string(void * const p, size_t length, const char ** const ptr, const char * const end, int error)
{
	size_t bcount;
	if (length == (size_t)-1) {
		bcount = coer_read_length(ptr, end, error);
		int unused = coer_read_uint8(ptr, end, error);
		bcount--;
		length = bcount * 8 - unused;
	}
	else {
		bcount = (length + 7) / 8;
	}
	if (bcount > 0) {
		cbuf_read(p, bcount, ptr, end, &error);
	}
	return length;
}

int coer_write_bit_string(const void * const p, size_t bitlength, char ** const ptr, const char * const end, int error)
{
	THROW_ERROR(EACCES);
	return -1;
}

coer_tag_t coer_read_tag(const char ** const ptr, const char * const end, int error)
{
	int x = coer_read_uint8(ptr, end, error);
	coer_tag_t c = (x >> 6);
	uint32_t t = x & 0x3F;
	if (t == 0x3F) {
		t = 0;
		do {
			x = coer_read_uint8(ptr, end, error);
			t = (t * 128) + (x & 0x7F);
		} while (x & 0x80);
	}
	return (c<<30) | t;
}

int coer_write_tag(coer_tag_t tag, char ** const ptr, const char * const end, int error)
{
	THROW_ERROR(EACCES);
	return -1;
}

size_t coer_read_sequence_of(void * const p, coer_read_fn read_fn, const char ** const ptr, const char * const end, int error)
{
	size_t count, i, ret=0;
	count = (size_t)coer_read_uint(ptr, end, error);
	for (i = 0; i < count; i++) {
		ret += read_fn((void*)i, p, ptr, end, error);
	}
	return ret;
}

char* _coer_write_sequenceof_count(size_t n, char** ptr, const char* end, int error)
{
	char* r = *ptr;
	coer_write_uint(n, ptr, end, error);
	return r;
}

void _coer_write_sequenceof_end(char* b, size_t n, const char* end, int error)
{
	size_t len = cintx_bytecount(n);
	if (len > 1) {
		if ((b + n + 1 + len) > end) {
			THROW_ERROR(ENOSPC);
		}
		memmove(b + 1 + len, b + 2, n);
		coer_write_uint(n, &b, b + 1 + len, error);
	}
	else {
		b[1] = (char)n;
	}
	*(b++) = (char)len;
}

char* _coer_write_open_type_length(size_t length, char** const ptr, const char* const end, int error)
{
	char* r = *ptr;
	coer_write_length(length, ptr, end, error);
	return r;
}

void _coer_write_open_type_end(char * b, char** const ptr, const char* const end, int error)
{
	char* e = *ptr;
	size_t len = e - b - 1;
	if (len > 127) {
		b[0] = (char)cintx_bytecount(len);
		if (e + b[0] > end) {
			THROW_ERROR(ENOSPC);
		}
		memmove(b + 1 + b[0], b + 1, len - 1);
	}
	else {
		b[0] = (char)len;
	}
}
