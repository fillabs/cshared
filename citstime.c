/*********************************************************************
 * This file is a part of FItsSec2 project: Implementation of 
 * IEEE Std. 1609.2,
 * ETSI TS 103 097 v1.3.1,
 * ETSI TS 102 941 v1.3.1
 * Copyright (C) 2020  Denis Filatov (denis.filatov()fillabs.com)

 * This file is NOT a free or open source software and shall not me used
 * in any way not explicitly authorized by the author.
*********************************************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "citstime.h"
#ifdef __GNUC__
#include <sys/time.h>
#endif

#define _SEC_IN_MINUTE 60L
#define _SEC_IN_HOUR 3600L
#define _SEC_IN_DAY 86400L
#define _SEC_IN_YEAR 31556952L

static const unsigned long _leap_moments[] = {
	1136073600,
	1230768000,
	1341100800,
	1435708800,
};

time_t addleapseconds(time_t t)
{
	int i;
	for (i = 0; i < sizeof(_leap_moments) / sizeof(_leap_moments[0]); i++){
		if (t < _leap_moments[i]) break;
	}
	return t + i;
}

time_t removeleapseconds(time_t t)
{
	int i;
	for (i = 0; i < sizeof(_leap_moments) / sizeof(_leap_moments[0]); i++){
		if (t < _leap_moments[i]) break;
		t--;
	}
	return t;
}

uint32_t mkitstime32(struct tm *tim_p)
{
	time_t ret = mktaitime(tim_p);
	if (ret > 0){
		ret -= ITS_UTC_EPOCH;
	}
	return (uint32_t)ret;
}

uint64_t mkitstime64(struct tm *tim_p)
{
	uint64_t ret = mktaitime64(tim_p);
	if (ret > 0){
		ret -= _itstime64from32(ITS_UTC_EPOCH);
	}
	return ret;
}

struct tm * itstime(struct tm *tim_p, uint32_t t)
{
	return taitime(tim_p, t + ITS_UTC_EPOCH);
}

struct tm * itstime64(struct tm *tim_p, uint64_t t)
{
	return taitime64(tim_p, t + _itstime64from32(ITS_UTC_EPOCH));
}

time_t mktaitime(struct tm *tim_p)
{
	time_t t = mkgmtime(tim_p);
	if (t >= 0){
		t = addleapseconds(t);
	}
	return t;
}

uint64_t mktaitime64(struct tm *tim_p)
{
	time_t t = mkgmtime(tim_p);
	if (t >= 0){
		t = addleapseconds(t);
	}
	return itstime64from32((uint32_t)t);
}

struct tm * taitime(struct tm *tim_p, time_t t)
{
	time_t st = removeleapseconds(t);
#ifdef _MSC_VER
	if (gmtime_s(tim_p, &st)) return tim_p;
	return NULL;
#else
	return gmtime_r(&st, tim_p);
#endif
}

struct tm * taitime64(struct tm *tim_p, uint64_t t)
{
	return taitime(tim_p, (uint32_t)removeleapseconds(itstime32from64(t)));
}

uint32_t unix2itstime32(time_t t)
{
	return ((uint32_t) addleapseconds(t)) - ITS_UTC_EPOCH;
}

uint32_t unix2itstime(time_t t)
{
	return ((uint32_t)addleapseconds(t)) - ITS_UTC_EPOCH;
}

uint64_t unix2itstime64(time_t t)
{
	t = addleapseconds(t) - ITS_UTC_EPOCH;
	return itstime64from32((uint32_t)t);
}

#ifdef _MSC_VER
struct timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
};
struct timespec {
	long    tv_sec;         /* seconds */
	long    tv_nsec;        /* and microseconds */
};
#endif
uint64_t timeval2itstime64(const struct timeval * tv)
{
    uint32_t ret = unix2itstime32(tv->tv_sec);
    return itstime64from32(ret) + tv->tv_usec;
}

uint64_t timespec2itstime64(const struct timespec * tv)
{
    uint32_t ret = unix2itstime32(tv->tv_sec);
    return itstime64from32(ret) + _itstime64fromnsec(tv->tv_nsec);
}

uint32_t timeval2itstime32(const struct timeval * tv)
{
    return unix2itstime32(tv->tv_sec);
}

static char _datebuf[8][16];
static int _datebufidx = 0;
const char * strgmtdate(time_t t)
{
	struct tm * tm;
	char * b = _datebuf[_datebufidx];
	_datebufidx = (_datebufidx + 1) & 7;

	tm = gmtime(&t);
	sprintf(b, "%u-%02u-%02u", 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday);
	return b;
}

const char * strtaidate(time_t t)
{
	return strgmtdate(removeleapseconds(t));
}

const char * stritsdate32(uint32_t t)
{
	return strtaidate(t + ITS_UTC_EPOCH);
}

const char * stritsdate64(uint64_t t)
{
	/* this is microseconds */
	return stritsdate32(itstime32from64(t));
}
const char* stritstime64(uint64_t t)
{
    return stritstime32(itstime32from64(t), (uint32_t)(t % 1000000));
}

const char* stritstime32(time_t t, uint32_t usec)
{
    return strtaitime(t + ITS_UTC_EPOCH, usec);
}

const char* strtaitime(time_t t, uint32_t usec)
{
    return strgmttime(removeleapseconds(t), usec);
}

static const char* _strtm2(struct tm* tm, uint32_t usec)
{
    char* b = _datebuf[_datebufidx];
    _datebufidx = (_datebufidx + 1) & 7;
    sprintf(b, "%02u:%02u:%02u.%06u", tm->tm_hour, tm->tm_min, tm->tm_sec, usec);
    return b;
}

const char* strgmttime(time_t t, uint32_t usec)
{
    return _strtm2(gmtime(&t), usec);
}

const char* strlocaltime(time_t t, uint32_t usec)
{
    return _strtm2(localtime(&t), usec);
}

int timeval_subtract(struct timeval* result, const struct timeval* x, const struct timeval* y)
{
    struct timeval yy = *y;

    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < yy.tv_usec) {
        int nsec = (yy.tv_usec - x->tv_usec) / 1000000 + 1;
        yy.tv_usec -= 1000000 * nsec;
        yy.tv_sec += nsec;
    }
    if (x->tv_usec - yy.tv_usec > 1000000) {
        int nsec = (yy.tv_usec - x->tv_usec) / 1000000;
        yy.tv_usec += 1000000 * nsec;
        yy.tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - yy.tv_sec;
    result->tv_usec = x->tv_usec - yy.tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < yy.tv_sec;
}

typedef uint32_t (fn_add_duration_32)(uint32_t t, uint32_t v);
typedef uint64_t (fn_add_duration_64)(uint64_t t, uint32_t v);
static uint32_t _time_32_add_error(uint32_t t, uint32_t v) {
	assert(NULL == "unknows time duration value"); 
	return 0;
}
static uint32_t _time_32_add_microseconds(uint32_t t, uint32_t v) { return t + v / 1000000; }
static uint32_t _time_32_add_milliseconds(uint32_t t, uint32_t v) { return t + v / 1000; }
static uint32_t _time_32_add_seconds(uint32_t t, uint32_t v) { return t + v; }
static uint32_t _time_32_add_minutes(uint32_t t, uint32_t v) { return t + v * 60; }
static uint32_t _time_32_add_hours(uint32_t t, uint32_t v) { return t + v * 3600; }
static uint32_t _time_32_add_60hours(uint32_t t, uint32_t v) { return t + v * 3600 * 60; }
static uint32_t _time_32_add_years(uint32_t t, uint32_t v) {
	return t + v * _SEC_IN_YEAR;
}
static uint64_t _time_64_add_microseconds(uint64_t t, uint32_t v) { return t + v / 10; }
static uint64_t _time_64_add_milliseconds(uint64_t t, uint32_t v) { return t + v * 100; }
static uint64_t _time_64_add_seconds(uint64_t t, uint32_t v) { return t + v * 1000000; }
static uint64_t _time_64_add_minutes(uint64_t t, uint32_t v) { return t + v * 60000000; }
static uint64_t _time_64_add_hours(uint64_t t, uint32_t v) { return t + v * 3600000000; }
static uint64_t _time_64_add_60hours(uint64_t t, uint32_t v) { return t + ((uint64_t)v) * 3600 * 60 * 1000000; }
static uint64_t _time_64_add_years(uint64_t t, uint32_t v) {
	return t + _itstime64from32(_SEC_IN_YEAR) * v;
}

static fn_add_duration_32 * const _duration_to_seconds_32[] = {
    _time_32_add_microseconds,
    _time_32_add_milliseconds,
    _time_32_add_seconds,
    _time_32_add_minutes,
    _time_32_add_hours,
    _time_32_add_60hours,
    _time_32_add_years,
    _time_32_add_error,
};
static fn_add_duration_64 * const _duration_to_seconds_64[] = {
    _time_64_add_microseconds,
    _time_64_add_milliseconds,
    _time_64_add_seconds,
    _time_64_add_minutes,
    _time_64_add_hours,
    _time_64_add_60hours,
    _time_64_add_years,
    (fn_add_duration_64 *)_time_32_add_error
};

uint32_t itstime32_add_duration(uint32_t t, duration_t dt, uint32_t v)
{
	return _duration_to_seconds_32[dt & 7](t, v);
}

uint64_t itstime64_add_duration(uint64_t t, duration_t dt, uint32_t v)
{
	return _duration_to_seconds_64[dt & 7](t, v);
}
