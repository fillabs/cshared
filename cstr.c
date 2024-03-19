/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2003 - 2015
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "cstr.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

size_t    cstrlen(const char * str)
{
	return str ? strlen(str) : 0;
}
size_t    cstrnlen(const char * str, size_t maxsize)
{
	size_t len = str ? strlen(str) : 0;
	return  len > maxsize ? maxsize : len;
}

char*  cstrend(const char * str)
{
	return (char*)(str ? str + strlen(str) : NULL);
}

int cstrequal(const char * s1, const char * s2)
{
	return (s1 == s2) || (s1 && s2 && 0 == strcmp(s1, s2));
}

int cstrnequal(const char* s1, const char* s2, size_t len)
{
	return (s1 == s2) || (s1 && s2 && 0 == strncmp(s1, s2, len));
}

char* cstrisprefix(const char * str, const char * prefix)
{
	size_t len;
	if (str == prefix){
		return (char*)(str ? str + strlen(str) : NULL);
	}
	if (str == NULL || prefix == NULL || 0 == *prefix){
		return (char*)str;
	}
	len = strlen(prefix);
	if (0 == memcmp(str, prefix, len)){
		return ((char*)str) + len;
	}
	return NULL;
}

char * cstrcpy(char * dst, const char * src)
{
	if(!dst) return (char*)0;
	size_t len = 0;
	if(src){
		len = strlen(src);
		if(len){
			memcpy(dst, src, len);
		}
	}
	dst[len]=0;
	return dst + len;
}

char * cstrchr(const char * str, int c)
{
	char * ret = (char*)0;
	if (str) {
		if (c == 0) ret = cstrend(str);
		else{
			ret = strchr(str, c);
			if (!ret) ret = cstrend(str);
		}
	}
	return ret;
}

char * cstrrchr(const char * str, int c)
{
	char * ret = (char*)0;
	if (str) {
		if (c == 0) ret = cstrend(str);
		else{
			ret = strrchr(str, c);
			if (!ret) ret = cstrend(str);
		}
	}
	return ret;
}

char * cstrcat(char * dst, const char * add)
{
	if (!dst) return (char*)0;
	char * end = dst + strlen(dst);
	return cstrcpy(end, add);
}

char * cstrncat(char * dst, size_t maxsize, const char * add)
{
	if (!dst) return (char*)0;
	char * end = dst + strlen(dst);
	return cstrncpy(end, maxsize-(end-dst), add);
}

/* copy up to maxsize characters from src to dst and return pointer to the next byte after the end */ 
char * cstrncpy(char * dst, size_t maxsize, const char * src)
{
	if (!dst) return (char*)0;
	size_t ms = maxsize;
	size_t len = 0;
	if (src && ms > 0){
		len = strlen(src);
		if (len > ms){
			len = ms;
		}
		if (len){
			memcpy(dst, src, len);
		}
	}
	dst[len] = 0;
	return dst + len;
}
/* This function is implemented as inline in cmem.h
char * cmemcpy(char * dst, const char * src, size_t length)
{
	if (!dst) return (char*)0;
	if (length){
		if (src){
			memcpy(dst, src, length);
		}
		else{
			memset(dst, 0, length);
		}
	}
	dst[length] = 0;
	return dst + length;
}
*/
/* copy up to maxsize characters from src to dst and return pointer to the next byte after the end */ 
char * cvstrncpy(char * dst, size_t maxsize, const char * ptr, ...)
{
	va_list ap;
	char * p = dst;
	const char * r = ptr;
	size_t ms = maxsize;
	if(ms > 0){
		va_start(ap, ptr);
		while(r){
			size_t len = strlen(r);
			if(len > ms) len = ms;
			memcpy(p, r, len);
			p  += len;
			ms -= len;
			r = va_arg(ap, const char*);
		}
		va_end(ap);
		*p = 0;
	}
	return p;
}

char * cstralloc(size_t size)
{
	return (char*)malloc((size+0x10)&(~0xF));
}


char * cstrpdups(char ** p, const char * str, size_t suffix)
{
	char * ret = NULL;
	size_t len = 0;
	if(str){
		len = strlen(str);
		ret = cstralloc(len + suffix + 1);
		if(ret){
			if(len) memcpy(ret, str, len);
			ret[len] = 0;
			*p = ret;
			return ret + len;
		}
	}
	*p = NULL;
	return NULL;
}

char * cstrpndups(char ** p, const char * str, size_t max_size, size_t suffix)
{
	char * ret = NULL;
	size_t len = 0;
	if(str){
		len = strnlen(str, max_size);
		ret = cstralloc(len + suffix);
		if(ret){
			if(len) memcpy(ret, str, len);
			ret[len] = 0;
			*p = ret;
			return ret + len;
		}
	}
	*p = NULL;
	return NULL;
}

char * cstrdups(const char * str, size_t suffix)
{
	char * ret = NULL;
	if(str){
		size_t len = strlen(str);
		ret = cstralloc(len + suffix + 1);
		if(ret){
			if(len) memcpy(ret, str, len);
			ret[len] = 0;
		}
	}
	return ret;
}

char * cstrdup(const char * str)
{
	return cstrdups(str, 0);
}

char * cstrndup(const char * str, size_t max_size)
{
	return cstrndups(str, max_size, 0);
}

char * cstrndups(const char * str, size_t max_size, size_t suffix)
{
	char * ret;
	cstrpndups(&ret, str, max_size, suffix);
	return ret;
}

char * cstrndupn(const char * str, size_t max_size, size_t suffix)
{
	char * ret = NULL;
	if(str){
		size_t len = strnlen(str, max_size-suffix);
		ret = cstralloc(len + suffix + 1);
		if(ret){
			if(len) memcpy(ret, str, len);
			ret[len] = 0;
		}
	}
	return ret;
}

char * cvstrdup(const char * ptr, ...)
{
	va_list ap;
	size_t len = 0;
	char *dst, *p;
	const char * r;
	
	if(!ptr) return (char*)ptr;

	// calculate size
	r = ptr;
	va_start(ap, ptr);
	while(r){
		len += strlen(r);
		r = va_arg(ap, const char*);
	}
	va_end(ap);
	
	p = dst = cstralloc(len+1);
	if(p){
		r = ptr;
		va_start(ap, ptr);
		while(r){
			len = strlen(r);
			memcpy(p, r, len);
			p += len;
			r = va_arg(ap, const char*);
		}
		va_end(ap);
		*p = 0;
	}
	return dst;
}

char * cstrnload(char * dst, size_t max_size, const pchar_t * path)
{
	FILE * f = pchar_fopen(path, _PCH("rb"));
	size_t len, rl;
	size_t ms = max_size;
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len > ms) len = ms;
	rl = fread(dst, 1, len, f);
	fclose(f);
	if ((int)rl < 0){
		return NULL;
	}
	if (len < ms)dst[len] = 0;
	return dst + len;
}

char * cstraload(char ** p, const pchar_t * path)
{
	char * ret = NULL;
	FILE * f = pchar_fopen(path, _PCH("rb"));
	size_t len;
	if (f){
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (len > 0){
			ret = malloc(len);
			if (ret){
				size_t ms = fread(ret, 1, len, f);
				if (ms < len){
					free(ret);
					*p = ret = NULL;
				}
				else{
					*p = ret;
					ret += len;
				}
			}
		}
		fclose(f);
	}
	return ret;
}

const char * cstrnsave(const char * data, size_t size, const pchar_t * path)
{
	const char * ret = NULL;
	FILE * f = pchar_fopen(path, _PCH("wb"));
	if (f){
		size_t l = fwrite(data, 1, size, f);
		if(l == size){
			ret = data + size;
		}
		fclose(f);
	}
	return ret;
}

const pchar_t * cstrlastpathelement(const pchar_t * str)
{
	const pchar_t * p = pchar_rchr(str, '/');
#ifdef WIN32
	const pchar_t * p2 = pchar_rchr(str, '\\');
	if(p<p2)p=p2;
#endif
	if(p == NULL) p = str;
	else p++;
	return p;
}

pchar_t * cstrpathextension(const pchar_t * str)
{
	const pchar_t * p;
	if (str){
		p = pchar_rchr(str, '.');
		if (!p) p = str + pchar_len(str);
	}
	else{
		p = str;
	}
	return (pchar_t*)p;
}

char* cstr_hex2bin(char* bin, size_t blen, const char* hex, size_t hlen)
{
	return cstr_hex2bin_ex(bin, blen, hex, hlen, " \t\n\r");
}

char* cstr_hex2bin_ex(char* bin, size_t blen, const char* hex, size_t hlen, const char * charstoskip)
{
	// check
	const char * h = hex;
	const char * e = hex+hlen;
	char * b = bin;
	int n = 0;
	while (h<e){
		char ch = *h++;
		if (strchr(charstoskip, ch)) continue;
		if (ch >= '0' && ch <= '9') continue;
		if (ch >= 'A' && ch <= 'F') continue;
		if (ch >= 'a' && ch <= 'f') continue;
		return NULL;
	}
	h = hex;
	while (h < e){
		char ch = *h++;
		if (ch >= '0' && ch <= '9') ch -= '0';
		else if (ch >= 'A' && ch <= 'F') ch -= 'A' - 0x0A;
		else if (ch >= 'a' && ch <= 'f') ch -= 'a' - 0x0A;
		else continue;
		if (!n){
			*b = ch;
		}
		else{
			char ch1 = *b;
			*b++ = (ch1 << 4) | ch;
		}
		n = !n;
	}
	if (n){
		char ch1 = *b;
		*b++ = (ch1 << 4);
		n = 0;
	}
	return b;
}

static const char* _hexDigits = "0123456789ABCDEF";
char * cstr_bin2hex(char * hex, size_t hlen, const char * bin, size_t blen)
{
	const unsigned char *b, *e;
	char * s;

	// sanity check
	if(hlen >=0 && hlen < blen * 2) return NULL;

	b = (const unsigned char *)bin;
	e = b + blen - 1;
	s = hex + blen * 2;
	if(s < hex + hlen) *s = 0;
	for (; b <= e; e--){
		*(--s) = _hexDigits[(*e) & 0xF];
		*(--s) = _hexDigits[(*e) >> 4];
	}
	return hex + blen * 2;
}

void *cmemmem(const void *haystack, size_t n, const void *needle, size_t m)
{
	const unsigned char *y = (const unsigned char *)haystack;
	const unsigned char *x = (const unsigned char *)needle;

	size_t j, k, l;

	if (m > n || !m || !n)
		return NULL;

	if (1 != m) {
		if (x[0] == x[1]) {
			k = 2;
			l = 1;
		} else {
			k = 1;
			l = 2;
		}

		j = 0;
		while (j <= n - m) {
			if (x[1] != y[j + 1]) {
				j += k;
			} else {
				if (!memcmp(x + 2, y + j + 2, m - 2) && x[0] == y[j])
					return (void *)&y[j];
				j += l;
			}
		}
	} else do {
		if (*y == *x)
			return (void *)y;
		y++;
	} while (--n);

	return NULL;
}

char * cstrnstr(const char *str, size_t size, const char* s)
{
	if(!str || !s || *s == 0) return (char*)str;
	return cmemmem(str, size, s, strlen(s));
}


int cstr_write(const char * const p, char ** const ptr, const char * const end, int * const error);
int cstr_read(char * const p, const char ** const ptr, const char * const end, int * const error);

int cstrn_write(const char * const p, size_t length, char ** const ptr, const char * const end, int * const error);
int cstrn_read(char * const p, size_t length, const char ** const ptr, const char * const end, int * const error);

#if defined( __GNUC__ )
#include <fnmatch.h>
int cstrmatch(const char * pattern, const char * str) {
	if(pattern){
		if(str)
			return (0 == fnmatch(pattern, str, 0));
		return 0;
	}
	return 1;
}
//#elif defined (_MSC_VER)
#else
int cstrmatch(const char * pattern, const char * str) {
	return cstrequal(pattern, str);
}
#endif