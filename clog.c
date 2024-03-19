/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##  Date      : 10.11.2005
##
##  Copyleft (c) 2003 - 2015
##  This code is provided under the CeCill-C license agreement.
######################################################################
*********************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "clog.h"
#include "cmem.h"
#include "cstr.h"
#include "copts.h"

#define CLOG_MAX_ITEM 5
typedef struct clog_item_t clog_item_t;

static void _clog_handler_file(int idx, clog_level_t level, void* user, const char* buf, size_t len);

#ifdef CLOG_PRINTF_HEX
#include <printf.h>
#include <errno.h>

static int printf_hex_string (FILE *stream, const struct printf_info *info, const void *const *args);
static int printf_hex_string_arginfo (const struct printf_info *info, size_t n, int *argtypes, int * size);
#endif

typedef struct clog_item_t {
    clog_cb_fn* h;
    void* f;
    clog_level_t level;
}clog_item_t;

static clog_item_t _out[CLOG_MAX_ITEM] = {
    {NULL}
};
static unsigned int _out_count = 0;
static clog_level_t _max_level = CLOG_NONE;

static const char * _clog_lnames[CLOG_LASTLEVEL] = {
    "NONE",
    "FATAL",
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
};

static char*  _buf = NULL;
static size_t _len = 0;

static void _clog_out_initialize(clog_level_t level)
{
    int idx = cfetch_and_inc(&_out_count);
    if (idx == 0) {
        _out[0].f = stderr;
        _out[0].h = _clog_handler_file;
        _out[0].level = level;
        _max_level = level;
    }
    else {
        _out_count--;
    }
    if (_buf == NULL) {
        _buf = malloc(_len=256);

#ifdef CLOG_PRINTF_HEX
        register_printf_specifier('H', printf_hex_string, printf_hex_string_arginfo);
#endif
    }
}

clog_level_t clog_level(unsigned int const out_index)
{
    if (out_index >= _out_count) {
        if (_out_count || out_index) {
            return (clog_level_t)-1;
        }
        _clog_out_initialize(CLOG_INFO);
    }
    return _out[out_index].level;
}

clog_level_t clog_set_level(unsigned int const out_index, clog_level_t level)
{
    if (((unsigned int)level) >= CLOG_LASTLEVEL) {
        return -1;
    }
    if (out_index >= _out_count) {
        if (_out_count || out_index) {
            return (clog_level_t)-1;
        }
        _clog_out_initialize(level);
    }
    if (level > _max_level) {
        _max_level = level;
    }
    return _out[out_index].level = level;
}

const char * clog_level_name(clog_level_t level)
{
    const char * ret = NULL;
    if (((unsigned int)level) < CLOG_LASTLEVEL)
        ret = _clog_lnames[level];
    return ret ? ret : CLOG_DEFAULT_LEVEL_NAME;
}

void clog_set_level_name(clog_level_t level, const char * const name)
{
    if(level < CLOG_LASTLEVEL)
        _clog_lnames[level] = name;
}

int  clog_set_file_output(void* const out, clog_level_t level)
{
    int idx = cfetch_and_inc(&_out_count);
    if (idx >= CLOG_MAX_ITEM) {
        _out_count--;
        return -1;
    }
    _out[idx].h = _clog_handler_file;
    _out[idx].f = out;
    _out[idx].level = level;
    if (level > _max_level) {
        _max_level = level;
    }
    return idx;
}


int  clog_set_cb_output(clog_cb_fn* const out, void* const user, clog_level_t level)
{
    int idx = cfetch_and_inc(&_out_count);
    if (idx >= CLOG_MAX_ITEM) {
        _out_count--;
        return -1;
    }
    _out[idx].h = out;
    _out[idx].f = user;
    _out[idx].level = level;
    if (level > _max_level) {
        _max_level = level;
    }
    return idx;
}

const char * clog_fprintf(void* const f, int const level, const char* format, ...)
{
    if (!_out_count) {
        _clog_out_initialize(CLOG_INFO);
    }
    if (level <= _max_level) {
        va_list ap;
        va_start(ap, format);
        size_t l = vsnprintf(_buf, _len, format, ap);
        va_end(ap);
        if (l == 0 || _buf[l - 1] != '\n') {
            if (l < _len - 1) {
                _buf[l++] = '\n';
                _buf[l] = 0;
            }
        }
        for (unsigned int i = 0; i < _out_count; i++) {
            if (level <= _out[i].level) {
                _out[i].h(i, level, _out[i].f, _buf, l);
            }
        }
        return _buf;
    }
    return NULL;
}

static  void _clog_handler_file(int idx, clog_level_t level, void* const user, const char* buf, size_t len)
{
    fwrite(buf, 1, len, (FILE*)user);
}

int clog_option(const copt_t* opt, const char* option, const copt_value_t* value)
{
    for (int level = 0; level < CLOG_LASTLEVEL; level++) {
        if (_clog_lnames[level][0] == toupper(value->v_str[0])) {
            const char* file = strchr(value->v_str, ':');
            if (file) {
                file++;
                if (cstrequal(file, "stdout")) {
                    clog_set_file_output(stdout, level);
                }
                else if (cstrequal(file, "stderr")) {
                    clog_set_file_output(stderr, level);
                }
                else {
                    const char* mode = "w";
                    if (*file == '+') {
                        file++;
                        mode = "a";
                    }
                    FILE* f = fopen(file, mode);
                    if (!f) {
                        perror(file);
                    }
                    else {
                        clog_set_file_output(f, level);
                    }
                }
                _clog_out_initialize(CLOG_INFO);
            }
            else {
                _clog_out_initialize(level);
                for (unsigned int idx = 0; idx < _out_count; idx++) {
                    _out[idx].level = level;
                }
            }
            return 0;
        }
    }
    fprintf(stderr, "%s: unknown log level\n", value->v_str);
    return -1;
}
#ifdef CLOG_PRINTF_HEX
static const char* _hexDigitsUp  = "0123456789ABCDEF";
static const char* _hexDigitsLow = "0123456789abcdef";
static int printf_hex_string (FILE *stream,
              const struct printf_info *info,
              const void *const *args)
{
    const unsigned char * p = *(const unsigned char**)(args[0]);
    int wlen = info->width;
    int slen = info->prec;
    if(wlen == 0){
        errno = EINVAL;
        return -1;
    }
    int ret = wlen;
    if(slen == -1 || slen > wlen){ // no source length given
        slen = wlen;
    }
    int pad = (info->pad) ? info->pad : ' ';
    if(0 == info->left){ 
        for(; wlen>slen; wlen--){
            fputc(pad, stream);
        }
    }
    const char * hd = (info->alt) ? _hexDigitsLow : _hexDigitsUp;

    const unsigned char * e = p + slen;
    
    for(; p < e; p++) {
        fputc( hd[(*p) & 0x0F], stream);
        fputc( hd[(*p) >> 4], stream);
    }
    
    if(info->left){ 
        for(; wlen>slen; wlen--){
            fputc(pad, stream);
        }
    }
    return ret;
}


static int
printf_hex_string_arginfo (const struct printf_info *info, size_t n, int *argtypes, int * size)
{
  if (n > 0){
    argtypes[0] = PA_STRING;
    size[0] = sizeof(void*);
  }
  return 1;
}
#endif
