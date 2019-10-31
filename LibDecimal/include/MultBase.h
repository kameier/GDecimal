#ifndef MULTBASE_H
#define MULTBASE_H

namespace MultBase
{
#define MUL_TOOM22_THRESHOLD 10
#define MUL_TOOM33_THRESHOLD 20 /* fixed */
#define MUL_TOOM44_THRESHOLD 24 /* fixed */

/**
 * @brief a = a + b
 * @param a
 * @param b
 * @return 返回是否溢出
 */

template <typename UINT_T, int MAX_LEN> inline UINT_T add_eq(UINT_T* a, const UINT_T* b)
{
    UINT8 t1 = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], b[i], a[i]);
    }
    return t1;
}

/**
 * @brief c = a + b
 * @param a
 * @param b
 * @return 返回是否溢出
 */
template <typename UINT_T, int MAX_LEN> inline UINT_T add(UINT_T* c, const UINT_T* a, const UINT_T* b)
{
    UINT8 t1 = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], b[i], c[i]);
    }
    return t1;
}
/*
 * a = a - b
 * 返回是否借位
 * 3 - 5 = 3 + 11 = 14   ...  14   借位 1
 * 5 - 3 = 5 + 13 = 18   ...   2   借位 0
 */
template <typename UINT_T, int MAX_LEN> inline UINT_T minus_eq(UINT_T* a, const UINT_T* b)
{
    UINT8 t1 = 1;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], ~(b[i]), a[i]);
    }
    return (t1 == 0) ? 1 : 0;
}

/**
 * @brief c = c[start:end] + u1
 * @param c
 * @param u1[MAX_LEN]
 * @param start
 */

template <typename UINT_T, int MAX_LEN> inline void _inline_add(UINT_T* c, const UINT_T* u1, int start)
{
    assert(start > 0);
    UINT8 carry = 0;
    for(int i = start + MAX_LEN - 1; i >= start; i--) {
        carry = _addcarry_base(carry, c[i], u1[i - start], c[i]);
    }
    for(int i = start - 1; i >= 0 && carry == 1; i--) {
        carry = _addcarry_base(carry, 0, c[i], c[i]);
    }
}

/*
 *  c = a * b
 *        11   12
 *    *   45   67
 *    =
 */
// template <typename UINT_T, int MAX_LEN>
// inline void Multiplication(UINT_T c[], UINT_T a[], UINT_T b[])
//{
//    assert(MAX_LEN % 2 == 0);
//    //    UINT_T* a_hi = a;
//    UINT_T* a_lo = &(a[MAX_LEN / 2]);
//    //    UINT_T* b_hi = b;
//    UINT_T* b_lo = &(b[MAX_LEN / 2]);
//
//    //    UINT_T u0[MAX_LEN] = {0};
//    UINT_T u1[MAX_LEN + 1] = {0};
//    UINT_T* u1_1 = u1 + 1;
//    //    UINT_T* u1_1 = &(u1[1]);
//    //    UINT_T* u0 = c;
//    UINT_T* u2 = c + MAX_LEN;
//    //    UINT_T* u2 = &(c[MAX_LEN]);
//    //    UINT_T u2[MAX_LEN] = {0};
//
//    Multiplication<UINT_T, (MAX_LEN / 2)>(u2, a_lo, b_lo);
//    //    Multiplication<UINT_T, (MAX_LEN / 2)>(u0, a_hi, b_hi);
//    Multiplication<UINT_T, (MAX_LEN / 2)>(c, a, b);
//
//    UINT_T aa[MAX_LEN / 2];
//    memcpy((void*)(aa), (void*)a, MAX_LEN / 2 * sizeof(UINT_T));
//    UINT_T bb[MAX_LEN / 2];
//    memcpy((void*)(bb), (void*)b, MAX_LEN / 2 * sizeof(UINT_T));
//
//    UINT_T a_carry = add_eq<UINT_T, MAX_LEN / 2>(aa, a_lo);
//    UINT_T b_carry = add_eq<UINT_T, MAX_LEN / 2>(bb, b_lo);
//
//    {
//        Multiplication<UINT_T, MAX_LEN / 2>(u1_1, aa, bb);
//        //    u1 += (a_carry)?bb<<MAX_LEN + (b_carry)?aa<<MAX_LEN;
//        UINT_T aa_carry = 0, bb_carry = 0;
//        if(a_carry == 1) { aa_carry = add_eq<UINT_T, MAX_LEN / 2>(u1_1, bb); }
//        if(b_carry == 1) { bb_carry = add_eq<UINT_T, MAX_LEN / 2>(u1_1, aa); }
//        _addcarry_base(a_carry * b_carry, aa_carry, bb_carry, u1[0]);
//        u1[0] -= minus_eq<UINT_T, MAX_LEN>(u1_1, c);
//        u1[0] -= minus_eq<UINT_T, MAX_LEN>(u1_1, u2);
//    }
//
//    // c = u0 + u1>>(MAX_LEN/2) + u2>>MAX_LEN
//    Karatsuba_inline_add<UINT_T, MAX_LEN>(c, u1);
//}

/**
 * @brief c = a * b
 * @param c[MAX_LEN + 1]
 * @param a[MAX_LEN]
 * @param b
 */

template <typename UINT_T, int MAX_LEN> inline void MultiUINT(UINT_T* c, const UINT_T* a, UINT_T b)
{
    if constexpr(MAX_LEN == 1) {
        c[1] = _mulx_base(a[0], b, c[0]);
        return;
    } else if constexpr(MAX_LEN == 2) {
        UINT_T lo;
        c[2] = _mulx_base(a[1], b, c[1]);
        lo = _mulx_base(a[0], b, c[0]);
        auto carry = _addcarry_base(0, lo, c[1], c[1]);
        carry = _addcarry_base(carry, 0, c[0], c[0]);
        return;
    } else if constexpr(MAX_LEN == 3) {
        UINT_T hi, lo;
        c[1] = _mulx_base(a[0], b, c[0]);
        c[3] = _mulx_base(a[2], b, c[2]);
        lo = _mulx_base(a[1], b, hi);
        auto carry = _addcarry_base(0, lo, c[2], c[2]);
        carry = _addcarry_base(carry, hi, c[1], c[1]);
        carry = _addcarry_base(carry, 0, c[0], c[0]);
        return;
    } else if constexpr(MAX_LEN > 3) {
        UINT_T hi, lo;
        c[MAX_LEN] = _mulx_base(a[MAX_LEN - 1], b, c[MAX_LEN - 1]);
        for(int i = MAX_LEN - 2; i >= 0; i--) {
            lo = _mulx_base(a[i], b, hi);
            auto carry = _addcarry_base(0, lo, c[i + 1], c[i + 1]);
            carry = _addcarry_base(carry, hi, c[i], c[i]);
        }
        return;
    } else {
        return;
    }
}

template <typename UINT_T, int MAX_LEN_a, int MAX_LEN_b>
inline void MUL_TOOM11_NoBaLance(UINT_T* c, const UINT_T* a, const UINT_T* b)
{
    UINT_T hi, lo;
    for(int i = MAX_LEN_b - 1; i >= 0; i--) {
        UINT_T bi = b[i];
        UINT_T cl = 0;
        int ij = i + MAX_LEN_a;
        for(int j = MAX_LEN_a - 1; j >= 0; j--, ij--) {
            lo = _mulx_base(a[j], bi, hi);
            // int ij = i + j + 1;
            cl = _addcarry_base(0, lo, cl, lo) + hi;
            cl += _addcarry_base(0, c[ij], lo, c[ij]);
        }
        c[ij] += cl;
    }
    return;
}
/**
 * @brief c = a * b
 * @param c[MAX_LEN * 2]
 * @param a[MAX_LEN]
 * @param b[MAX_LEN]
 */

template <typename UINT_T, int MAX_LEN> inline void MUL_TOOM11(UINT_T* c, const UINT_T* a, const UINT_T* b)
{
    if constexpr(MAX_LEN > 3) {
        if constexpr(0) {
            UINT_T d[MAX_LEN * 2] = { 0 };
            for(int i = MAX_LEN - 1; i >= 0; i--) {
                UINT_T bi = b[i];
                int ij = i + MAX_LEN;
                for(int j = MAX_LEN - 1; j >= 0; j--) {
                    UINT_T hi, lo;
                    lo = _mulx_base(a[j], bi, hi);
                    // int ij = i + j + 1;
                    // d[ij] += _addcarry_base(0, c[ij], lo, c[ij]);
                    d[ij] += _addeq_carry_base(lo, c[ij]);
                    ij--;
                    d[ij] += _addcarry_base(0, c[ij], hi, c[ij]);
                    // d[ij] += _addeq_carry_base(hi, c[ij]);
                }
            }
            auto carry = 0;
            UINT_T* d1 = d + 1;
            for(int n = MAX_LEN * 2 - 2; n >= 0; n--) {
                // carry = _addcarry_base(carry, c[n], d1[n], c[n]);
                d1[n] += carry;
                c[n] += d1[n];
                carry = (c[n] < d1[n]);
            }
            //    DEBUG_COUT(carry);
            return;
        } else {
            for(int i = MAX_LEN - 1; i >= 0; i--) {
                UINT_T bi = b[i];
                UINT_T cl = 0;
                int ij = (i + MAX_LEN);
                for(int j = MAX_LEN - 1; j >= 0; j--, ij--) {
                    UINT_T hi, lo;
                    lo = _mulx_base(a[j], bi, hi);
                    // int ij = i + j + 1;
                    cl = _addeq_carry_base(cl, lo) + hi;
                    cl += _addeq_carry_base(lo, c[ij]);
                }
                c[ij] = cl;
            }
            return;
        }
    } else if constexpr(MAX_LEN == 1) {
        c[1] = _mulx_base(a[0], b[0], c[0]);
        return;
    } else if constexpr(MAX_LEN == 2) {
        UINT_T hi_1, hi_2;
        UINT_T lo_1, lo_2;
        c[1] = _mulx_base(a[0], b[0], c[0]);
        lo_1 = _mulx_base(a[0], b[1], hi_1);
        lo_2 = _mulx_base(a[1], b[0], hi_2);
        c[3] = _mulx_base(a[1], b[1], c[2]);
        //    c[2] += lo[0] + lo[1];
        UINT8 carry;
        carry = _addcarry_base(0, c[2], lo_1, c[2]);
        carry += _addcarry_base(0, c[2], lo_2, c[2]);
        //    c[1] += hi[0] + hi[1];
        carry = _addcarry_base(0, c[1], carry, c[1]);
        carry += _addcarry_base(0, c[1], hi_1, c[1]);
        carry += _addcarry_base(0, c[1], hi_2, c[1]);
        //    c[0] = hi[0];
        c[0] += carry;
        return;
    } else if constexpr(MAX_LEN == 3) {
        MultiUINT<UINT_T, 3>(c, a, b[0]);
        UINT_T u2[4] = { 0 };
        MultiUINT<UINT_T, 3>(u2, a, b[2]);
        MultBase::_inline_add<UINT_T, 4>(c, u2, 2);
        UINT_T u1[4] = { 0 };
        MultiUINT<UINT_T, 3>(u1, a, b[1]);
        MultBase::_inline_add<UINT_T, 4>(c, u1, 1);
        return;
    } else {
        return;
    }
}

/**
 * @brief c = a * b Karatsuba
 * @param c[MAX_LEN * 2]
 * @param a[MAX_LEN]
 * @param b[MAX_LEN]
 */

template <typename UINT_T, int MAX_LEN>
//__attribute((always_inline))
inline void MUL_TOOM22(UINT_T* c, const UINT_T* a, const UINT_T* b)
{
    if constexpr(MAX_LEN < MUL_TOOM22_THRESHOLD) {
        MUL_TOOM11<UINT_T, MAX_LEN>(c, a, b);
        return;
    } else if constexpr(MAX_LEN >= MUL_TOOM22_THRESHOLD && MAX_LEN % 2 == 0) {
        UINT_T* a_lo = MAX_LEN / 2 + (UINT_T*)a;
        UINT_T* b_lo = MAX_LEN / 2 + (UINT_T*)b;

        UINT_T u1[MAX_LEN + 1] = { 0 };
        UINT_T* u1_1 = u1 + 1;
        UINT_T* u2 = c + MAX_LEN;

        MUL_TOOM22<UINT_T, (MAX_LEN / 2)>(u2, a_lo, b_lo);
        MUL_TOOM22<UINT_T, (MAX_LEN / 2)>(c, a, b);

        UINT_T aa[MAX_LEN / 2] = { 0 };
        UINT_T bb[MAX_LEN / 2] = { 0 };
        auto a_carry = MultBase::add<UINT_T, MAX_LEN / 2>(aa, a, a_lo);
        auto b_carry = MultBase::add<UINT_T, MAX_LEN / 2>(bb, b, b_lo);

        MUL_TOOM22<UINT_T, MAX_LEN / 2>(u1_1, aa, bb);
        auto aa_carry = 0, bb_carry = 0;
        if(a_carry == 1) {
            aa_carry = MultBase::add_eq<UINT_T, MAX_LEN / 2>(u1_1, bb);
        }
        if(b_carry == 1) {
            bb_carry = MultBase::add_eq<UINT_T, MAX_LEN / 2>(u1_1, aa);
        }
        //        _addcarry_base(a_carry & b_carry, aa_carry, bb_carry, u1[0]);
        u1[0] = aa_carry + bb_carry + (a_carry & b_carry);
        u1[0] -= minus_eq<UINT_T, MAX_LEN>(u1_1, c);
        u1[0] -= minus_eq<UINT_T, MAX_LEN>(u1_1, u2);

        // c = u0 + u1>>(MAX_LEN/2) + u2>>MAX_LEN
        MultBase::_inline_add<UINT_T, MAX_LEN + 1>(c, u1, MAX_LEN / 2 - 1);
        return;
    } else {
        return;
    }
}

/**
 * @brief c = a * b
 * @param c[MAX_LEN * 2]
 * @param a[MAX_LEN]
 * @param b[MAX_LEN]
 */
template <typename UINT_T, int MAX_LEN> inline void Multiplication(UINT_T* c, const UINT_T* a, const UINT_T* b)
{
    if constexpr(MAX_LEN < MUL_TOOM22_THRESHOLD) {
        MUL_TOOM11<UINT_T, MAX_LEN>(c, a, b);
        return;
    } else if constexpr(MAX_LEN >= MUL_TOOM22_THRESHOLD && MAX_LEN % 2 == 0) {
        MUL_TOOM22<UINT_T, MAX_LEN>(c, a, b);
        return;
    } else if constexpr(MAX_LEN > MUL_TOOM22_THRESHOLD && MAX_LEN % 2 == 1) {
        MUL_TOOM22<UINT_T, MAX_LEN - 1>(&c[2], &a[1], &b[1]);
        UINT_T hi, lo;
        UINT_T u1[MAX_LEN] = { 0 };
        UINT_T u2[MAX_LEN] = { 0 };
        MultiUINT<UINT_T, MAX_LEN - 1>(u1, &a[1], b[0]);
        MultBase::_inline_add<UINT_T, MAX_LEN>(c, u1, 1);
        MultiUINT<UINT_T, MAX_LEN - 1>(u2, &b[1], a[0]);
        MultBase::_inline_add<UINT_T, MAX_LEN>(c, u2, 1);
        lo = _mulx_base(a[0], b[0], hi);
        auto carry = _addcarry_base(0, lo, c[1], c[1]);
        _addcarry_base(carry, hi, c[0], c[0]);
        return;
    } else {
        return;
    }
}

template <typename UINT_T, int MAX_LEN> inline void Mult_eq(UINT_T* a, const UINT_T* b)
{
    UINT_T c[MAX_LEN * 2] = { 0 };
    MultBase::Multiplication<UINT_T, MAX_LEN>(c, a, b);
    UINT8 carry = (c[MAX_LEN + 1] >> (sizeof(UINT_T) * 8 - 1));
    for(int i = MAX_LEN; i > 0 && carry; i--) {
        carry = _addcarry_base(carry, 0, c[i], c[i]);
    }
    memcpy(a, c + 1, MAX_LEN * sizeof(UINT_T));
}

template <typename UINT_T, int MAX_LEN> inline int lzcnt(const UINT_T a[])
{
    int count = 0;
    int i = 0;
    for(i = 0; i < MAX_LEN; ++i) {
        if(a[i] == 0) {
            count += (8 * sizeof(UINT_T));
        } else {
            count += lzcnt_base(a[i]);
            return count;
        }
    }
    return count;
}

template <typename UINT_T, int MAX_LEN> inline int nzcnt(const UINT_T a[])
{
    return 8 * sizeof(UINT_T) * MAX_LEN - MultBase::lzcnt<UINT_T, MAX_LEN>(a);
}

template <typename UINT_T> inline UINT_T ReciprocalInitValue(UINT_T a1)
{
    constexpr UINT_T UINT_L_MAX = (~UINT_T(0));
    UINT_T AA = (UINT_L_MAX - UINT_L_MAX / 17) - a1 / 2 + a1 / 34;
    //    UINT_T hi;
    //    _mulx_base(a1, a1 / 99 * 32, hi);
    // P = [98 -80 32]/99;
    //    UINT_T AA = (UINT_L_MAX - UINT_L_MAX / 99) - a1 / 99 * 80 + hi;
    // UINT_T AA = UINT_L_MAX * 0.957106781186547 - a1 / 2;
    int count = 0;
    for(UINT_T B = 2; B > 1; count++) {
        _mulx_base(a1, AA, B);
        auto ch = _addcarry_base(0, AA, B, B);
        if(ch) {
            _mulx_base(B, AA, B);
            AA -= B;
        } else {
            _mulx_base(1 + ~B, AA, B);
            AA += B;
        }
    }
    if(count > 5) {
        DEBUG_COUT(count);
    }
    return AA;
}

template <typename UINT_T, int MAX_LEN> inline int Negative(UINT_T* AA)
{
    UINT8 t1 = 1;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
    }
    return t1;
}
/**
 * @brief 返回迭代次数
 * @param a
 * @param A  A = 1/a
 * @param AA 临时存储空间
 * @return
 */
template <typename UINT_T, int MAX_LEN> inline int Reciprocal(const UINT_T* a, UINT_T* A)
{
    if constexpr(MAX_LEN == 1) {
        A[0] = 1;
        return 1;
    } else if constexpr(MAX_LEN == 2) {
        A[0] = 0;
        A[1] = ReciprocalInitValue(a[1]);
        return 1;
    } else if constexpr(MAX_LEN > 2 && (MAX_LEN % 2) == 0) {
        int count = MultBase::Reciprocal<UINT_T, MAX_LEN / 2>(a, A);
        UINT_T AA[MAX_LEN * 2] = { 0 };
        UINT_T BB[MAX_LEN * 2] = { 0 };
        //        int cnt;
        MultBase::Multiplication<UINT_T, MAX_LEN>(AA, A, a);
        if(AA[1] == 0) {
            UINT8 t1 = 1;
            for(int i = MAX_LEN; i > 1; i--) {
                t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
            }
            //            cnt = MultBase::lzcnt<UINT_T, MAX_LEN>(AA);
            MultBase::Multiplication<UINT_T, MAX_LEN>(BB, AA + 1, A);
            MultBase::add_eq<UINT_T, MAX_LEN>(A, BB + 1);
        } else {
            AA[1] = 0;
            //            cnt = MultBase::lzcnt<UINT_T, MAX_LEN>(AA);
            MultBase::Multiplication<UINT_T, MAX_LEN>(BB, AA + 1, A);
            MultBase::minus_eq<UINT_T, MAX_LEN>(A, BB + 1);
        }
        //        DEBUG_COUT(cnt);
        //        DEBUG_COUT(MAX_LEN);
        return count + 1;
    } else if constexpr(MAX_LEN > 2 && (MAX_LEN % 2) != 0) {
        constexpr int A_MAX_LEN = pow(2, ilog2<MAX_LEN>::value);
        return MultBase::Reciprocal<UINT_T, A_MAX_LEN>(a, A);
    } else {
        return 0;
    }
}

// template <>
// inline void Multiplication<UINT16, 2>(UINT16* p, UINT16* a, UINT16* b)
//{
//    UINT32 aa = a[0];
//    aa = (aa << 16) + a[1];
//    UINT32 bb = b[0];
//    bb = (bb << 16) + b[1];
//    UINT32 hi, lo;
//    lo = _mulx_base(aa, bb, hi);
//    p[0] = hi >> 16;
//    p[1] = hi;
//    p[2] = lo >> 16;
//    p[3] = lo;
//}
//
// template <>
// inline void Multiplication<UINT32, 2>(UINT32* p, UINT32* a, UINT32* b)
//{
//    UINT64 aa = a[0];
//    aa = (aa << 32) + a[1];
//    UINT64 bb = b[0];
//    bb = (bb << 32) + b[1];
//    UINT64 hi, lo;
//    //    multiply(hi, lo, aa, bb);
//    lo = _mulx_base(aa, bb, hi);
//    p[0] = hi >> 32;
//    p[1] = hi;
//    p[2] = lo >> 32;
//    p[3] = lo;
//}
//
// template <>
// inline void Multiplication<UINT64, 2>(UINT64* c, UINT64* a, UINT64* b)
//{
//    UINT64 hi_1, hi_2;
//    UINT64 lo_1, lo_2;
//    c[1] = _mulx_base(a[0], b[0], c[0]);
//    lo_1 = _mulx_base(a[0], b[1], hi_1);
//    lo_2 = _mulx_base(a[1], b[0], hi_2);
//    c[3] = _mulx_base(a[1], b[1], c[2]);
//    //    c[2] += lo[0] + lo[1];
//    UINT8 carry;
//    carry = _addcarry_base(0, c[2], lo_1, c[2]);
//    carry += _addcarry_base(0, c[2], lo_2, c[2]);
//    //    c[1] += hi[0] + hi[1];
//    carry = _addcarry_base(0, c[1], carry, c[1]);
//    carry += _addcarry_base(0, c[1], hi_1, c[1]);
//    carry += _addcarry_base(0, c[1], hi_2, c[1]);
//    //    c[0] = hi[0];
//    c[0] += carry;
//}

} // namespace MultBase
#endif // MultBase_H
