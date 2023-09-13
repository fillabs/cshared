#define COER_ENABLE_EXCEPTIONS
#include "../casn_oer.h"
#include "../e4c_lite.h"
#include "../cstr.h"

#include <stdio.h>
#include <assert.h>

static void _test_uint(uint64_t v, size_t len, const char * tpl){
    char buf[256];
    char * b = buf;
    uint64_t r;
    printf("write uint(0x%lx): ", v);
    coer_write_uint(v, &b, buf+sizeof(buf), 0);
    if(b-buf != len + 1) {
        printf("wrong length\n");
    }else if(buf[0] != len){
        printf("length mismatch\n");
    }else if(memcmp(buf+1, tpl, len)){
        printf("value mismatch\n");
    }else {
        printf("OK\n");
    }

    b = buf;
    printf("read uint(0x%lx): ", v);
    r = coer_read_uint((const char**)&b, buf+sizeof(buf), 0);
    if(b-buf != len + 1) {
        printf("wrong length\n");
    }else if(r != v){
        printf("value mismatch\n");
    }else {
        printf("OK\n");
    }
}

static void _write_body(char ** ptr, const char * end, size_t len) {
    for(size_t i=0; i<len; i++){
        coer_write_uint8(i, ptr, end, 0);
    }
}

static int _check_body(const char * b, size_t len) {
    size_t i;
    for(i = 0; i<len; i++){
        if(((uint8_t*)b)[i] != (uint8_t)i){
            printf("data mismatch at pos %ld\n", i);
            return 0;
        }
    }
    return 1;
}

static void _test_sequence_of(size_t l, size_t cntlen, const char * cnttpl)
{
    char * buf = malloc(l+cntlen);
    const char * end = buf + l + cntlen;
    char * b = buf;
    int err = 0;

    printf("write sequence (%ld) of uint8: ", l);
    coer_write_sequenceof_do(seqof, &b, end, 0){
        _write_body(&b, end, l);
        coer_write_sequenceof_end(seqof,l,&b,end,0);
    }
    if((b-buf) != (l + cntlen)){
        printf("wrong size\n");
    }else if(memcmp(buf, cnttpl, cntlen)){
        printf("count mismatch\n");
    }else if(_check_body(buf + cntlen, l)){
        printf("OK\n");
    }

    b = buf;
    err = 0;
    printf("read sequence (%ld) of uint8: ", l);
    coer_read_sequence_of_do(seqof, (const char**)&b, end, 0){
        if(seqof_sequence_index == 0){
            if(b-buf != cntlen){
                printf("wrong count size\n");
                err = 1;
                break;
            }
            if(seqof_sequence_count != l){
                err = 1;
                printf("count mismatch\n");
                break;
            }
        }
        uint8_t v = coer_read_uint8((const char**)&b, end, 0);
        if(v != (uint8_t)seqof_sequence_index){
            err = 1;
            printf("data mismatch at pos %ld\n", seqof_sequence_index);
            break;
        }
    }
    if(!err){
        printf("OK\n");
    }
    free(buf);
}

static void _test_opentype(size_t il, size_t l, size_t cntlen, const char * cnttpl)
{
    size_t alen = 9 + ((l>il)?l:il);
    char * buf = malloc(alen);
    const char * end = buf + alen;
    char * b = buf;

    printf("write opentype (%ld) : ", l);
    coer_write_open_type(optype, il, &b, end, 0){
        _write_body(&b, end, l);
    }    
    if((b-buf) != (l + cntlen)){
        printf("wrong size: %ld ws %ld\n", (b-buf), (l + cntlen));
    }else if(memcmp(buf, cnttpl, cntlen)){
        printf("length mismatch\n");
    }else if(_check_body(buf + cntlen, l)){
        printf("OK\n");
    }

    b = buf;
    printf("read opentype (%ld): ", l);
    coer_read_open_type_do(otype, (const char**)&b, end, 0){
        if(b-buf != cntlen){
            printf("wrong count size\n");
        }else if(otype_size != l){
            printf("count mismatch\n");
        }else if(_check_body(b, otype_size)){
            printf("OK\n");
        }
    }
    free(buf);
}

int main(int argc, char ** argv){

    try{
        _test_uint(0,            1, "\x0");
        _test_uint(0xFF,         1, "\xFF");
        _test_uint(0x100,        2, "\x01\x00");
        _test_uint(0xFFFF,       2, "\xFF\xFF");
        _test_uint(0xFFFFFFFF,   4, "\xFF\xFF\xFF\xFF");
        _test_uint(0x0100000000, 5, "\x01\x00\x00\x00\x00");

        _test_sequence_of(0,        2, "\x01\x00");
        _test_sequence_of(1,        2, "\x01\x01");
        _test_sequence_of(0xFF,     2, "\x01\xFF");
        _test_sequence_of(0x100,    3, "\x02\x01\x00");
        _test_sequence_of(0xFFFF,   3, "\x02\x0FF\xFF");
        _test_sequence_of(0x10000,  4, "\x03\x01\x00\x00");

        _test_opentype(0,   0,     1, "\x00");
        _test_opentype(255, 0,     1, "\x00");
        _test_opentype(0, 127,      1, "\x7F");
        _test_opentype(0, 128,      2, "\x81\x80");
        _test_opentype(0x100, 0x100,    3, "\x82\x01\x00");
        _test_opentype(0, 0x100,    3, "\x82\x01\x00");

    }
    catch(RuntimeException){
        printf("exception\n");
    }
    return 0;
}
