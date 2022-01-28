/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2020
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/
#ifndef cbyteswap_h
#define cbyteswap_h

#if defined(__GNUC__)
# define cint64_swap(X) __builtin_bswap64(X)
# define cint32_swap(X) __builtin_bswap32(X)
# define cint16_swap(X) __builtin_bswap16(X)
# define __PACKED__(DECL) DECL __attribute__((__packed__))
#elif defined (_MSC_VER)
# define cint64_swap(X) _byteswap_uint64(X)
# define cint32_swap(X) _byteswap_ulong (X)
# define cint16_swap(X) _byteswap_ushort(X)
# define __ORDER_LITTLE_ENDIAN__ 1
# define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
# define __PACKED__(DECL) __pragma( pack(push, 1) ) DECL __pragma( pack(pop))
#else
uint64_t cint64_swap(const uint64_t);
uint32_t cint32_swap(const uint32_t);
uint16_t cint16_swap(const uint16_t);
#endif
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# define cint64_hton(X) cint64_swap(X)
# define cint64_ntoh(X) cint64_swap(X)
# define cint32_hton(X) cint32_swap(X)
# define cint32_ntoh(X) cint32_swap(X)
# define cint16_hton(X) cint16_swap(X)
# define cint16_ntoh(X) cint16_swap(X)
# define cint64_lsb(X)  ((uint8_t)(((uint64_t)(X))>>56))
# define cint32_lsb(X)  ((uint8_t)(((uint32_t)(X))>24))
# define cint24_lsb(X)  ((uint8_t)(((uint32_t)(X))>16))
# define cint64_lsb3(X) ((uint32_t)(((uint64_t)(X))>>40))
# define cint32_lsb3(X) ((uint32_t)(((uint64_t)(X))>>8))
#else
# define cint64_hton(X) (X)
# define cint64_ntoh(X) (X)
# define cint32_hton(X) (X)
# define cint32_ntoh(X) (X)
# define cint16_hton(X) (X)
# define cint16_ntoh(X) (X)
# define cint64_lsb(X)  ((uint8_t)((X)&0xFF))
# define cint32_lsb(X)  ((uint8_t)((X)&0xFF))
# define cint24_lsb(X)  ((uint8_t)((X)&0xFF))
# define cint64_lsb3(X) ((uint32_t)((X)&0xFFFFFF))
# define cint32_lsb3(X) ((uint32_t)((X)&0xFFFFFF))
#endif /* __BYTE_ORDER__ */

#endif
