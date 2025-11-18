#ifndef citstime_h
#define citstime_h

#include <time.h>
#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif

    #define ITS_UTC_EPOCH 1072915200

    #define _itstime32from64(t) ((uint32_t)(t/1000000))
    static inline uint32_t itstime32from64(uint64_t t){
		return _itstime32from64(t);
	}
    #define _itstime64from32(t)    (((uint64_t)t)*1000000) 
    #define _itstime64fromsec(t)   _itstime64from32(t)
    #define _itstime64frommsec(t)  (((uint64_t)t)*1000)
    #define _itstime64fromusec(t)  ((uint64_t)t)
    #define _itstime64fromnsec(t)  (((uint64_t)t)/1000)
    static inline uint64_t itstime64from32(uint32_t t){
		return _itstime64from32(t);
	}
    #define itstime32to64(t) itstime64from32(t)
    #define itstime64to32(t) itstime32from64(t)

	time_t mktaitime(struct tm *tim_p);
	time_t addleapseconds(time_t t);
	time_t removeleapseconds(time_t t);
	struct tm * itstime(struct tm *tim_p, uint32_t t);
	struct tm * itstime64(struct tm *tim_p, uint64_t t);
	struct tm * taitime(struct tm *tim_p, time_t t);
	struct tm * taitime64(struct tm *tim_p, uint64_t t);

	#define mktaitime32(X) ((uint32_t)mktaitime(X))
	uint32_t mkitstime32(struct tm *tim_p);
	uint32_t unix2itstime32(time_t t);
		uint64_t mktaitime64(struct tm *tim_p);
	uint64_t mkitstime64(struct tm *tim_p);
	uint32_t unix2itstime32(time_t t);
	uint64_t unix2itstime64(time_t t);
	#ifndef WIN32
	#define mkgmtime(TM) timegm(TM)
	#endif

	int      timeval_subtract(struct timeval* result, const struct timeval* x, const struct timeval* y);
	uint32_t timeval2itstime32(const struct timeval * tv);    
	uint64_t timeval2itstime64(const struct timeval * tv);
	#define  timespec2itstime32(tv) timeval2itstime32((const struct timeval *)tv)
	uint64_t timespec2itstime64(const struct timespec * tv);
	static inline uint32_t time32from64(uint64_t t){
		return (uint32_t)(t/1000000);
	}

	const char * stritsdate32(uint32_t t);
	const char * stritsdate64(uint64_t t);
	const char * strtaidate(time_t t);
	const char * strgmtdate(time_t t);
    const char * stritstime64(uint64_t t);
    const char * stritstime32(time_t t, uint32_t usec);
    const char * strtaitime(time_t t, uint32_t usec);
    const char * strgmttime(time_t t, uint32_t usec);
    const char * strlocaltime(time_t t, uint32_t usec);

	typedef enum {
		dt_microseconds,
		dt_milliseconds,
		dt_seconds,
		dt_minutes,
		dt_hours,
		dt_sixtyHours,
		dt_years,

		__dt_max
	}duration_t;

	uint32_t itstime32_add_duration(uint32_t t, duration_t dt, uint32_t v);
	uint64_t itstime64_add_duration(uint64_t t, duration_t dt, uint32_t v);

#ifdef __cplusplus
}
#endif
#endif
