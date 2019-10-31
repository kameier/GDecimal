#ifndef BASE_H
#define BASE_H

#include <assert.h>

#include <algorithm>
#include <basetsd.h>
#include <cmath>
#include <immintrin.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <vector>

//#ifndef __x86_64__
//#define __x86_64__
//#endif
#ifdef __clang__
/*code specific to clang compiler*/
//#define __GLIBCXX_BITSIZE_INT_N_0 128
//#define __GLIBCXX_TYPE_INT_N_0 __int128
#include <intrin.h>
typedef unsigned __int128 UINT128;
#elif __GNUC__
/*code for GNU C compiler */
#include <intrin.h>
typedef unsigned __int128 UINT128;
#elif _MSC_VER
/*usually has the version number in _MSC_VER*/
/*code specific to MSVC compiler*/
#include <intrin.h>
#elif __BORLANDC__
/*code specific to borland compilers*/
#elif __MINGW32__
/*code specific to mingw compilers*/
#endif

/**
 * @brief sum_out = src1 + src2 + c_in
 * @param c_in
 * @param src1
 * @param src2
 * @param sum_out
 * @return c_out 溢出
 */
inline UINT8 _addcarry_base(const UINT8 c_in, const UINT16 src1, const UINT16 src2, UINT16& sum_out)
{
    UINT32 sum = c_in;
    sum += src1;
    sum += src2;
    sum_out = (UINT16)sum;
    return (UINT8)(sum >> 16);
}

// extern unsigned char _addcarry_u32(unsigned char c_in, unsigned int src1, unsigned int src2, unsigned int *sum_out);
// unsigned char _addcarryx_u64 (unsigned char c_in, unsigned __int64 a, unsigned __int64 b, unsigned __int64 * out)

/**
 * @brief sum_out = src1 + src2 + c_in
 * @param c_in
 * @param src1
 * @param src2
 * @param sum_out
 * @return c_out 溢出
 */
inline UINT8 _addcarry_base(const UINT8 c_in, const UINT32 src1, const UINT32 src2, UINT32& sum_out)
{
#ifdef __clang__
    /*code specific to clang compiler*/
    return _addcarry_u32(c_in, src1, src2, &sum_out);
#elif __GNUC__
    /*code for GNU C compiler */
    return _addcarry_u32(c_in, src1, src2, &sum_out);
#elif _MSC_VER
    /*usually has the version number in _MSC_VER*/
    /*code specific to MSVC compiler*/
    UINT64 sum = c_in;
    sum += src1;
    sum += src2;
    sum_out = (UINT32)sum;
    return (sum >> 32);
#endif
}

/**
 * @brief sum_out = src1 + src2 + c_in
 * @param c_in
 * @param src1
 * @param src2
 * @param sum_out
 * @return c_out 溢出
 */
inline UINT8 _addcarry_base(const UINT8 c_in, const UINT64 src1, const UINT64 src2, UINT64& sum_out)
{
#ifdef __clang__
    /*code specific to clang compiler*/
    return _addcarry_u64(c_in, src1, src2, &sum_out);
#elif __GNUC__
    /*code for GNU C compiler */
    return _addcarry_u64(c_in, src1, src2, &sum_out);
#elif _MSC_VER
    /*usually has the version number in _MSC_VER*/
    /*code specific to MSVC compiler*/
    return _addcarry_u64(c_in, src1, src2, &sum_out);
#elif __BORLANDC__
    /*code specific to borland compilers*/
#elif __MINGW32__
    /*code specific to mingw compilers*/
    UINT128 __res = (UINT128)a + b;
    sum_out = (UINT64)(__res >> 64);
    return (UINT64)__res;
#endif
}

inline UINT8 _subborrow_base(const UINT8 c_in, const UINT32 src1, const UINT32 src2, UINT32& sum_out)
{
    return _subborrow_u32(c_in, src1, src2, &sum_out);
}

inline UINT8 _subborrow_base(const UINT8 c_in, const UINT64 src1, const UINT64 src2, UINT64& sum_out)
{
    return _subborrow_u64(c_in, src1, src2, &sum_out);
}

/**
 * @brief sum_out += src1
 * @param src1
 * @param sum_out
 * @return c_out 溢出
 */
template <typename T> inline bool _addeq_carry_base(const T src1, T& sum_out)
{
    sum_out += src1;
    return (sum_out < src1);
}
// inline void multiply(UINT64& rhi, UINT64& rlo, UINT64 a, UINT64 b)
//{
//#ifdef __clang__
//    /*code specific to clang compiler*/
//#elif __GNUC__
//    /*code for GNU C compiler */
//    __asm__("mulq  %[b]\n" : "=d"(rhi), "=a"(rlo) : "1"(a), [b] "r"(b));
////        rlo = _umul128(a, b, &rhi);
//#elif _MSC_VER
//    /*usually has the version number in _MSC_VER*/
//    /*code specific to MSVC compiler*/
//    rlo = _mulx_u64(a, b, &rhi);
//#endif
//}
//
// inline void multiply(UINT32& rhi, UINT32& rlo, UINT32 a, UINT32 b)
//{
//#ifdef __clang__
//    /*code specific to clang compiler*/
//#elif __GNUC__
//    /*code for GNU C compiler */
//    __asm__("MUL  %[b]\n" : "=d"(rhi), "=a"(rlo) : "1"(a), [b] "r"(b));
//#elif _MSC_VER
//    /*usually has the version number in _MSC_VER*/
//    /*code specific to MSVC compiler*/
//    rlo = _mulx_u32(a, b, &rhi);
//    /*
//        __asm
//        {
//                mov eax, a;
//                mov ebx, b;
//                mul ebx;
//                mov rhi edx;
//                mov rlo eax;
//        }
//        */
//#endif
//}
//
//#ifdef __clang__
///*code specific to clang compiler*/
//#elif __GNUC__
///*code for GNU C compiler */
// inline void multiply(UINT16& rhi, UINT16& rlo, UINT16 a, UINT16 b)
//{
//    __asm__("MUL  %[b]\n" : "=d"(rhi), "=a"(rlo) : "1"(a), [b] "r"(b));
//}
//#endif
// UINT32 _addcarry_uint32(UINT32 c_in, UINT32 src1, UINT32 src2, UINT32& sum_out)
//{
//    return _addcarryx_u32(c_in, src1, src2, &sum_out);
//}
// extern unsigned char _addcarry_u32(unsigned char c_in, unsigned int src1, unsigned int src2, unsigned int *sum_out);
inline UINT16 _mulx_base(UINT16 a, UINT16 b, UINT16& hi)
{
    UINT32 p = a;
    p *= b;
    hi = p >> 16;
    return (UINT16)p;
}

inline UINT32 _mulx_base(UINT32 a, UINT32 b, UINT32& hi)
{
    UINT64 __res = UINT64(a) * b;
    hi = __res >> 32;
    return (UINT32)__res;
}

// unsigned __int64 _mulx_u64 (unsigned __int64 a, unsigned __int64 b, unsigned __int64* hi)

inline UINT64 _mulx_base(const UINT64 a, const UINT64 b, UINT64& hi)
{
#ifdef __clang__
    /*code specific to clang compiler*/
    UINT64 lo;
    __asm__("mulq  %[b]\n" : "=d"(hi), "=a"(lo) : "1"(a), [b] "r"(b));
    return lo;
#elif __GNUC__
    /*code for GNU C compiler */
    // mulq S : R[rdx]:R[rax]← S * R[rax] (64位*64位无符号整数）
    UINT64 lo;
    __asm__("mulq  %[b]\n" : "=d"(hi), "=a"(lo) : "1"(a), [b] "r"(b));
    return lo;
#elif _MSC_VER
    /*usually has the version number in _MSC_VER*/
    /*code specific to MSVC compiler*/
    return _umul128(a, b, &hi);
#elif __BORLANDC__
    /*code specific to borland compilers*/
#elif __MINGW32__
    /*code specific to mingw compilers*/
#endif
}

//__attribute__((target("lzcnt"))) inline UINT16 lzcnt_base(UINT16 a) { return __lzcnt16(a); }
//__attribute__((target("lzcnt"))) inline UINT32 lzcnt_base(UINT32 a) { return _lzcnt_u32(a); }
//__attribute__((target("lzcnt"))) inline UINT64 lzcnt_base(UINT64 a) { return _lzcnt_u64(a); }
template <typename T> inline T lzcnt_base(T t)
{
    T cnt = 0;
    const int L = 8;
    for(int i = 8 * (int)sizeof(T) - L; i >= 0; i -= L) {
        UINT8 ch = (t >> i);
        if(ch == 0) {
            cnt += L;
        } else {
            cnt += lzcnt_base(ch);
            return cnt;
        }
    }
    return 8 * sizeof(T);
}

// template <>
// inline UINT8 lzcnt_base(UINT8 t)
//{
//    UINT8 i;
//    for(i = 0; i < 9; i++)
//    {
//        if((t >> i) == 0)
//            return 8 - i;
//    }
//    return 0;
//}
template <> inline UINT8 lzcnt_base(UINT8 x)
{
    UINT8 n = 0;
    // 1111 0000
    if((x & 0xF0) == 0) {
        n += 4;
        x <<= 4;
    }
    // 1100 0000
    if((x & 0xC0) == 0) {
        n += 2;
        x <<= 2;
    }
    // 1000 0000
    if((x & 0x80) == 0) {
        n += 1;
    }
    return n;
}

template <> inline UINT16 lzcnt_base(UINT16 x)
{
    UINT16 n = 0;
    if((x & 0xFF00) == 0) {
        n += 8;
        x <<= 8;
    }
    if((x & 0xF000) == 0) {
        n += 4;
        x <<= 4;
    }
    if((x & 0xC000) == 0) {
        n += 2;
        x <<= 2;
    }
    if((x & 0x8000) == 0) {
        n += 1;
    }
    return n;
}

template <> inline UINT32 lzcnt_base(UINT32 x)
{
    UINT32 n = 0;
    if((x & 0xFFFF0000) == 0) {
        n += 16;
        x <<= 16;
    }
    if((x & 0xFF000000) == 0) {
        n += 8;
        x <<= 8;
    }
    if((x & 0xF0000000) == 0) {
        n += 4;
        x <<= 4;
    }
    if((x & 0xC0000000) == 0) {
        n += 2;
        x <<= 2;
    }
    if((x & 0x80000000) == 0) {
        n += 1;
    }
    return n;
}

/**
 * @macro lzcnt
 * @brief leading zero count (count #continuous zeros from MSb)
 */
#ifdef __clang__
/*code specific to clang compiler*/
#elif __GNUC__
/*code for GNU C compiler */
template <> inline UINT64 lzcnt_base(UINT64 x)
{
#ifdef __LZCNT__
    return _lzcnt_u64(x);
#else
    // #warning "lzcnt instruction is not enabled."
    if(x == 0) {
        return (64);
    } else {
        UINT64 res;
        __asm__("bsrq %1, %0" : "=r"(res) : "r"(x));
        return (63 - res);
    }
#endif
}
#elif _MSC_VER
/*usually has the version number in _MSC_VER*/
/*code specific to MSVC compiler*/
template <> inline UINT64 lzcnt_base(UINT64 x)
{
    UINT64 n = 0;
    UINT64 _2e32 = 0x100000000U;
    if(x < _2e32) {
        UINT32 xx = x;
        n = 32 + lzcnt_base(xx);
    } else {
        UINT32 xx = x >> 32;
        n = lzcnt_base(xx);
    }
    return n;
}
#elif __BORLANDC__
/*code specific to borland compilers*/
#elif __MINGW32__
/*code specific to mingw compilers*/
#endif

template <typename T> inline T nzcnt_base(T t)
{
    return 8 * sizeof(T) - lzcnt_base(t);
}

template <typename T> inline T bitscan_base(T t)
{
    T i;
    for(i = 0; t; i++) {
        t &= (t - 1);
    }
    return i;
}

/**
 * @macro tzcnt
 * @brief trailing zero count (count #continuous zeros from LSb)
 */
#ifdef __clang__
/*code specific to clang compiler*/
#elif __GNUC__
/*code for GNU C compiler */
inline uint64_t tzcnt(uint64_t n)
{
#ifdef __BMI__
    /** immintrin.h is already included */
    return ((uint64_t)_tzcnt_u64(x));
#else
    // #warning "tzcnt instruction is not enabled."
#ifdef __POPCNT__
    return (popcnt(~n & (n - 1)));
#else
    if(n == 0) {
        return (64);
    } else {
        UINT64 res;
        __asm__("bsfq %1, %0" : "=r"(res) : "r"(n));
        return (res);
    }
#endif
#endif
}
#elif __BORLANDC__
/*code specific to borland compilers*/
#elif __MINGW32__
/*code specific to mingw compilers*/
#endif

inline UINT16 _divided_base(UINT16& hi, UINT16 lo, UINT16 a)
{
    UINT32 __res = (UINT32(hi) << 16) + lo;
    hi = __res % a;
    return __res / a;
}

inline UINT32 _divided_base(UINT32& hi, UINT32 lo, UINT32 a)
{
    UINT64 __res = (UINT64(hi) << 32) + lo;
    hi = __res % a;
    return __res / a;
}

inline UINT32 _divided_base(UINT32& hi, UINT32 lo, UINT16 a)
{
    assert(hi < a);
    //    assert(a < 0x10000);
    UINT32 q = (~UINT32(0)) / a;
    UINT32 r = (~UINT32(0)) % a;

    q = hi * q + lo / a;
    r = hi * r + hi + lo % a;
    //    auto c = _addcarry_base(0, hi * r, hi, r);
    //    c = _addcarry_base(c, r, lo % a, r);
    //    if(c == 1) { DEBUG_COUT((int)c); }
    hi = r % a;
    return q + r / a;
}

inline UINT64 _divided_base(UINT64& hi, UINT64 lo, UINT64 a)
{
#ifdef __clang__
    /*code specific to clang compiler*/
    assert(hi < a);
    UINT128 __res = (UINT128(hi) << 64) + lo;
    hi = __res % a;
    return __res / a;
#elif __GNUC__
    /*code for GNU C compiler */
    assert(hi < a);
    UINT128 __res = (UINT128(hi) << 64) + lo;
    hi = __res % a;
    return __res / a;
#elif _MSC_VER
    /*usually has the version number in _MSC_VER*/
    /*code specific to MSVC compiler*/
    assert(hi < a);
    UINT64 qq = (~UINT64(0)) / a;
    //	UINT64 rr = (~UINT64(0)) % a + 1;
    // r = hi * rr + lo % a;
    UINT64 q = 0;
    UINT64 hhi = hi;
    while(hhi > 0) {
        UINT64 q0 = qq * hhi;
        q += q0;
        UINT64 res_hi, res_lo;
        res_lo = _mulx_base(q0, a, res_hi);
        auto c = _addcarry_base(1, lo, ~res_lo, lo);
        _addcarry_base(c, hhi, ~res_hi, hhi);
    }
    q += lo / a;
    hi = lo % a;
    return q;
#elif __BORLANDC__
    /*code specific to borland compilers*/
#elif __MINGW32__
    /*code specific to mingw compilers*/
    assert(hi < a);
    UINT128 __res = (UINT128(hi) << 64) + lo;
    hi = __res % a;
    return __res / a;
#endif
}
// std::imaxdiv_t div(std::intmax_t x, std::intmax_t y);

inline UINT64 _divided_base(UINT64& hi, UINT64 lo, UINT32 a)
{
    assert(hi < a);
    //    assert(a < 0x100000000);
    UINT64 q = (~UINT64(0)) / a;
    UINT64 r = (~UINT64(0)) % a;

    q = hi * q + lo / a;
    r = hi * r + hi + lo % a;
    //    auto c = _addcarry_base(0, hi * r, hi, r);
    //    c = _addcarry_base(c, r, lo % a, r);
    //    if(c == 1) { DEBUG_COUT((int)c); }
    hi = r % a;
    return q + r / a;
}

inline void SetN1(unsigned short* pBuf, int n)
{
    (*pBuf) |= 1 << n;
}

inline void SetN0(unsigned short* pBuf, int n)
{
    (*pBuf) &= ~(1 << n);
}

inline void SetN1(unsigned char* pBuf, int n)
{
    (*pBuf) |= 1 << n;
}

inline void SetN0(unsigned char* pBuf, int n)
{
    (*pBuf) &= ~(1 << n);
}

inline int Get1Bit(unsigned char buf, int n)
{
    return (buf >> n) & 0x01;
}

inline int Get1Bit(unsigned short buf, int n)
{
    return (buf >> n) & 0x01;
}

inline int Get2Bit(unsigned char buf, int n)
{
    return (buf >> n) & 0x03;
}

inline int Get2Bit(unsigned short buf, int n)
{
    return (buf >> n) & 0x03;
}

inline void Set2Bit(unsigned short* pBuf, int n, int m)
{
    (*pBuf) |= m << n;
}
#endif // BASE_H
