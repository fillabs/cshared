/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2003 - 2015
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/
#ifndef CSTR_H
#define CSTR_H
#include <string.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#define cisspace(X) isspace((int)(X))
#define cisdigit(X) isdigit((int)(X))
#define cisalnum(X) isalnum((int)(X))

size_t    cstrlen(const char * str);
size_t    cstrnlen(const char * str, size_t maxsize);
/* return pointer to the end of line*/
char*  cstrend(const char * str);
/* copy src to dst and return pointer to the next byte after the end */
char * cstrcpy(char * dst, const char * src);

/* copy up to maxsize characters from src to dst and return pointer to the next byte after the end */ 
char * cstrncpy(char * dst, size_t maxsize, const char * src);

char * cstrcat(char * dst, const char * add);
char * cstrncat(char * dst, size_t maxsize, const char * add);

/* copy up to maxsize characters from parameters to dst and return pointer to the next byte after the end */ 
char * cvstrncpy(char * dst, size_t maxsize, const char * ptr, ...);

/* return nonzero if strings are equals*/
int cstrequal(const char * s1, const char * s2);
int cstrnequal(const char* s1, const char* s2, size_t len);
char* cstrisprefix(const char * str, const char * prefix);

char * cstrchr(const char * str, int c);
char * cstrrchr(const char * str, int c);
char * cstrnstr(const char *str, size_t size, const char* s);

/* allocate copy of the str */ 
char * cstralloc(size_t size);
char * cstrdup(const char * str);
char * cstrdups(const char * str, size_t suffix);
char * cstrndup(const char * str, size_t max_size);
char * cstrndups(const char * str, size_t max_size, size_t suffix);
char * cstrpdups(char ** p, const char * str, size_t suffix);
char * cstrpndups(char ** p, const char * str, size_t max_size, size_t suffix);

/* allocate new str and collect all paramaters */ 
char * cvstrdup(const char * ptr, ...);

int cstr_write(const char * const p, char ** const ptr, const char * const end, int * const error);
int cstr_read (char * const p, const char ** const ptr, const char * const end, int * const error);

int cstrn_write(const char * const p, size_t length, char ** const ptr, const char * const end, int * const error);
int cstrn_read (char * const p, size_t length, const char ** const ptr, const char * const end, int * const error);

char* cstr_hex2bin_ex(char* bin, size_t blen, const char* hex, size_t hlen, const char * charstoskip);
char* cstr_hex2bin(char* bin, size_t blen, const char* hex, size_t hlen);
char * cstr_bin2hex(char * hex, size_t hlen, const char * bin, size_t blen);

/* file access functions */
#ifndef PCHAR_T_DEFINED
#if defined(UNICODE) && ( defined(_WIN32) || defined(_WIN64) )
#define PCHAR16BITS
#include <wchar.h>
typedef wchar_t pchar_t;
#define _PCH(X) L##X
#else
typedef char pchar_t;
#define _PCH(X) X
#endif
#endif

/* load file content to the buffer and return pointer to the next byte after the buffer */
char * cstrnload(char * dst, size_t max_size, const pchar_t * path);
/* load file content to the new allocated buffer, assign it to p and return pointer to the next byte after the buffer */
char * cstraload(char ** p, const pchar_t * path);
/* store data in the file. returns pointer to the end of the data.*/
const char * cstrnsave(const char * data, size_t size, const pchar_t * path);

/* return the last element of the path */
const pchar_t * cstrlastpathelement(const pchar_t * str);
pchar_t * cstrpathextension(const pchar_t * str);

int cstrmatch(const char * pattern, const char * str);

#ifdef PCHAR16BITS
#define pchar_len(S)      wcslen(S)
#define pchar_ncpy(D,S,N) wcsncpy(D,S,N)
#define pchar_cpy(D,S)    wcscpy(D,S)
#define pchar_rchr(S,C)   wcsrchr(S,C)
#define pchar_fopen(P,M) _wfopen(P, M)
#define pchar_main(A,V)   wmain(A,V)
#else
#define pchar_len(S)      strlen(S)
#define pchar_ncpy(D,S,N) strncpy(D,S,N)
#define pchar_cpy(D,S)    strcpy(D,S)
#define pchar_rchr(S,C)   strrchr(S,C)
#define pchar_fopen(P,M)  fopen(P, M)
#define pchar_main(A,V)   main(A,V)
#endif
#define pchar_alloc(S) ((pchar_t*)malloc(sizeof(pchar_t)*((S)+1)))

#ifdef _MSC_VER
#define cPrefixUint64 "%016I64"
#else 
#define cPrefixUint64 "%016l"
#endif

#ifdef __cplusplus
}
#endif
#endif
