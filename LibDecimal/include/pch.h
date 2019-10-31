#ifndef PCH_H
#define PCH_H

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

//#define MYDEBUG
#ifdef MYDEBUG
#define DEBUG_COUT(x) \
    std::cout << std::endl << std::dec << __FUNCTION__ << "() L" << __LINE__ << ":" << #x << " = " << x << std::endl
#else
#define DEBUG_COUT(x) ((void)0)
#endif

#ifdef NDEBUG
#define advise(_Expression) ((void)0)
#else /* !defined (NDEBUG) */
#define advise(_Expression)                                                                                   \
    (void)((!!(_Expression)) ||                                                                               \
        (printf_s("\n  In File:%s, Function: %s(), Line: %d \n\n    Advise: %s \n\n", __FILE__, __FUNCTION__, \
            __LINE__, #_Expression)))
#endif /* !defined (NDEBUG) */

template <typename T> inline T max(const T a, const T b)
{
    return (a > b) ? a : b;
}
template <typename T> inline T min(const T a, const T b)
{
    return (a < b) ? a : b;
}

typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;

template <int N> struct ilog2 {
    static constexpr int value = 1 + ilog2<N / 2>::value;
};

template <> struct ilog2<3> {
    static constexpr int value = 1;
};
template <> struct ilog2<2> {
    static constexpr int value = 1;
};
template <> struct ilog2<1> {
    static constexpr int value = 0;
};

#include "Base.h"
#include "FixPointData.h"
#include "MultBase.h"

template <typename UINT_T, int MAX_LEN> using FixPointData = FixPointVector<UINT_T, MAX_LEN>;
// using FixPointData = FixPointArray<UINT_T, MAX_LEN>;

#include "FixPointOperator.h"
#include "FixPointReciprocal.h"

#include "Decimal.h"
//#include "CTX.h"
#include "DecimalConvert.h"
typedef Decimal<UINT64, 128> decimal;
#include "DecimalMath.h"

#endif // PCH_H
