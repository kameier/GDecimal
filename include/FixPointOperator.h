#ifndef FixPointOperator_H
#define FixPointOperator_H

namespace FixPointOperator
{
/*
左移函数
{0,1,2,3,4} -> {1,2,3,4,0}
*/
template <typename UINT_T, int MAX_LEN> inline void left_shift(FixPointData<UINT_T, MAX_LEN>& a, size_t ls)
{
    if(ls >= MAX_LEN * (8 * sizeof(UINT_T))) {
        for(int i = 0; i < MAX_LEN; ++i) {
            a[i] = 0;
        }
        return;
    }
    if(ls >= (8 * sizeof(UINT_T))) {
        int m = ls / (8 * sizeof(UINT_T));
        for(int i = 0; i < MAX_LEN - m; ++i) {
            a[i] = a[i + m];
        }
        for(int i = MAX_LEN - m; i < MAX_LEN - 1; ++i) {
            a[i] = 0;
        }
        ls = ls % (8 * sizeof(UINT_T));
    }
    if(ls == 0) {
        return;
    }
    UINT_T t = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        UINT_T u = a[i];
        a[i] = (u << ls) + t;
        t = u >> ((8 * sizeof(UINT_T)) - ls);
    }
}

/*
    右移函数
    {0,1,2,3,4} -> {0,0,1,2,3}
*/
template <typename UINT_T, int MAX_LEN> inline void right_shift(FixPointData<UINT_T, MAX_LEN>& a, size_t rs)
{
    UINT_T t = 0;
    if(rs >= MAX_LEN * (8 * sizeof(UINT_T))) {
        for(int i = 0; i < MAX_LEN; ++i) {
            a[i] = 0;
        }
        return;
    }
    int m = rs / (8 * sizeof(UINT_T));
    for(int i = MAX_LEN - 1; i >= m; i--) {
        a[i] = a[i - m];
    }
    for(int i = m - 1; i >= 0; i--) {
        a[i] = 0;
    }
    rs = rs % (8 * sizeof(UINT_T));
    if(rs == 0) {
        return;
    }
    for(int i = 0; i < MAX_LEN; ++i) {
        UINT_T u = a[i];
        a[i] = (u >> rs) + t;
        t = u << ((8 * sizeof(UINT_T)) - rs);
    }
}

/*
    a = a / b
    r = a % b
    返回 r
*/
template <typename UINT_T, int MAX_LEN> inline UINT_T divide_eq(FixPointData<UINT_T, MAX_LEN>& a, const UINT_T b)
{
    UINT_T hi = 0;
    for(int i = 0; i < MAX_LEN; i++) {
        a[i] = _divided_base(hi, a[i], b);
    }
    return hi;
}

template <typename UINT_T, int MAX_LEN> inline UINT_T shift_divide_eq(FixPointData<UINT_T, MAX_LEN>& a, const UINT_T b)
{
    assert(b > a[0]);
    UINT_T hi = a[0];
    if constexpr(std::is_same<UINT_T, UINT64>::value) {
        constexpr UINT_T _2e32 = UINT_T(1) << (32);
        if(b < _2e32) {
            UINT32 bb = b;
            int i;
            for(i = 1; i < MAX_LEN; i++) {
                a[i - 1] = _divided_base(hi, a[i], bb);
            }
            a[i - 1] = _divided_base(hi, 0, bb);
            return hi;
        }
    } else if constexpr(std::is_same<UINT_T, UINT32>::value) {
        constexpr UINT_T _2e16 = UINT_T(1) << (16);
        if(b < _2e16) {
            UINT16 bb = b;
            int i;
            for(i = 1; i < MAX_LEN; i++) {
                a[i - 1] = _divided_base(hi, a[i], bb);
            }
            a[i - 1] = _divided_base(hi, 0, bb);
            return hi;
        }
    } else {
    }
    int i;
    for(i = 1; i < MAX_LEN; i++) {
        a[i - 1] = _divided_base(hi, a[i], b);
    }
    a[i - 1] = _divided_base(hi, 0, b);
    return hi;
}

template <typename UINT_T, int MAX_LEN> inline char shift_divide_eq_By_10(FixPointData<UINT_T, MAX_LEN>& a)
{
    constexpr char b = 10;
    constexpr UINT_T qq = (~UINT_T(0)) / b;
    constexpr UINT_T rr = (~UINT_T(0)) % b + 1;

    UINT_T hi = a[0];
    int i;
    for(i = 1; i < MAX_LEN; i++) {
        UINT_T q = hi * qq + a[i] / b;
        UINT_T r = hi * rr + a[i] % b;
        a[i - 1] = q + r / b;
        hi = r % b;
    }
    {
        UINT_T r = hi * rr;
        a[i - 1] = hi * qq + r / b;
        hi = r % b;
    }
    return hi;
}

template <typename UINT_T, int MAX_LEN> inline char divide_eq_By_10(FixPointData<UINT_T, MAX_LEN>& a)
{
    constexpr char b = 10;
    constexpr UINT_T qq = (~UINT_T(0)) / b;
    constexpr UINT_T rr = (~UINT_T(0)) % b + 1;

    UINT_T hi = a[0];
    for(int i = 1; i < MAX_LEN; i++) {
        UINT_T q = hi * qq + a[i] / b;
        UINT_T r = hi * rr + a[i] % b;
        a[i] = q + r / b;
        hi = r % b;
    }
    return hi;
}

///*
//    c = a * b
//    返回
//*/
// template <typename UINT_T, int MAX_LEN>
// inline void
// multi(const FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b, FixPointData<UINT_T, MAX_LEN>&
// c)
//{
//    assert(a.size() == b.size());
//    int idx_end = MAX_LEN - 1;
//    FixPointData<UINT_T, MAX_LEN> p = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
//    FixPointData<UINT_T, MAX_LEN> d = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
//    UINT_T hi;
//    UINT_T dst;
//    UINT_T ai;
//    for(int i = idx_end; i >= 0; i--)
//    {
//        ai = a[i];
//        for(int j = idx_end - i; j >= 0; --j)
//        {
//            //            dst = _mulx_base(ai, b[j], hi);
//            multiply(hi, dst, ai, b[j]);
//            int ij = i + j;
//            if(ij > 0)
//            {
//                d[ij] += _addcarry_base(0, p[ij], dst, p[ij]);
//                --ij;
//                d[ij] += _addcarry_base(0, p[ij], hi, p[ij]);
//            }
//            else
//            {
//                _addcarry_base(0, p[ij], dst, p[ij]);
//            }
//        }
//    }
//    UINT_T d_count = 0; // d[count]
//    UINT_T p_count = 0; // p[count]
//    // i + j = count
//    for(int i = 0; i < MAX_LEN; ++i)
//    {
//        dst = _mulx_base(a[i], b[MAX_LEN - i], hi);
//        d_count += _addcarry_base(0, p_count, dst, p_count);
//        d[idx_end] += _addcarry_base(0, p[idx_end], hi, p[idx_end]);
//    }
//    UINT8 carry = 0;
//    if(p_count >= (UINT_T(1) << (8 * sizeof(UINT_T) - 1)))
//    {
//        carry = 1;
//    }
//    carry = _addcarry_base(carry, p[idx_end], d_count, c[idx_end]);
//    for(int n = idx_end - 1; n >= 0; --n)
//    {
//        carry = _addcarry_base(carry, p[n], d[n + 1], c[n]);
//    }
//}

/*
    a = a * b
    返回 void
*/
template <typename UINT_T, int MAX_LEN>
inline void multi_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    assert(a.size() == b.size());
    int idx_end = MAX_LEN - 1;
    FixPointData<UINT_T, MAX_LEN> p = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    FixPointData<UINT_T, MAX_LEN> d = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    UINT_T hi;
    UINT_T dst;
    UINT_T ai;
    for(int i = idx_end; i >= 0; --i) {
        ai = a[i];
        for(int j = idx_end - i; j >= 0; --j) {
            dst = _mulx_base(ai, b[j], hi);
            //            multiply(hi, dst, ai, b[j]);
            int ij = i + j;

            if(ij > 0) {
                d[ij] += _addcarry_base(0, p[ij], dst, p[ij]);
                --ij;
                d[ij] += _addcarry_base(0, p[ij], hi, p[ij]);
            } else {
                _addcarry_base(0, p[ij], dst, p[ij]);
            }
        }
    }
    UINT_T d_MAX_LEN = 0; // d[MAX_LEN]
    UINT_T p_MAX_LEN = 0; // p[MAX_LEN]
    // i + j = count
    for(int i = 0; i < MAX_LEN; ++i) {
        dst = _mulx_base(a[i], b[MAX_LEN - i], hi);
        d_MAX_LEN += _addcarry_base(0, p_MAX_LEN, dst, p_MAX_LEN);
        d[idx_end] += _addcarry_base(0, p[idx_end], hi, p[idx_end]);
    }
    UINT8 carry = 0;
    if(p_MAX_LEN >= (UINT_T(1) << (8 * sizeof(UINT_T) - 1))) {
        carry = 1;
    }
    carry = _addcarry_base(carry, p[idx_end], d_MAX_LEN, a[idx_end]);
    for(int n = idx_end - 1; n >= 0; --n) {
        carry = _addcarry_base(carry, p[n], d[n + 1], a[n]);
    }
}

template <typename UINT_T, int MAX_LEN>
inline void multi_all_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    FixPointData<UINT_T, MAX_LEN* 2 - 1> p = FixPointData<UINT_T, MAX_LEN * 2 - 1>(a.size() + b.size() - 1, 0);
    FixPointData<UINT_T, MAX_LEN* 2> d = FixPointData<UINT_T, MAX_LEN * 2>(a.size() + b.size(), 0);

    for(int i = a.size() - 1; i >= 0; i--) {
        UINT_T ai = a[i];
        for(int j = b.size() - 1; j >= 0; j--) {
            UINT_T hi;
            UINT_T dst = _mulx_base(ai, b[j], hi);
            int ij = i + j;

            if(ij > 0) {
                d[ij] += _addcarry_base(0, p[ij], dst, p[ij]);
                --ij;
                d[ij] += _addcarry_base(0, p[ij], hi, p[ij]);
            } else {
                _addcarry_base(0, p[ij], dst, p[ij]);
            }
        }
    }
    UINT8 carry = 0;
    //    if (p[c.size()] > (1LL << 31))
    //    {
    //        carry = 1;
    //    }
    for(int n = a.size() - 1; n >= 0; n--) {
        carry = _addcarry_base_UINT_L(carry, p[n], d[n + 1], a[n]);
    }
}

/*
    a = a * b
    返回 进位carry
*/
template <typename UINT_T, int MAX_LEN> inline UINT_T multi_eq(FixPointData<UINT_T, MAX_LEN>& a, UINT_T b)
{
    UINT_T carry = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        UINT_T lo, hi;
        lo = _mulx_base(a[i], b, hi);
        a[i] = lo + carry;
        carry = hi + (a[i] < lo);
    }
    return carry;
}

template <typename UINT_T, int MAX_LEN>
inline bool _compare(const FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    assert(a.size() == b.size());
    for(int i = 0; i < MAX_LEN; ++i) {
        if(a[i] > b[i]) {
            return 1;
        }
        if(a[i] < b[i]) {
            return -1;
        }
    }
    return 0;
}

/*
    c = a - b
    返回是否溢出
*/
template <typename UINT_T, int MAX_LEN>
inline int
minus(const FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b, FixPointData<UINT_T, MAX_LEN>& c)
{
    //    assert(a.size() == b.size() && a.size() == c.size());
    // c = FixPointData<UINT_T, MAX_LEN>(a.size(), 0);

    UINT8 t1 = 1;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], ~(b[i]), c[i]);
    }
    // 4 - 2 = 4 + 6 = 10 = 2,  t1 = 1
    // 4 - 6 = 4 + 2 = 6  = -2, t1 = 0
    if(t1 == 0) {
        UINT8 t2 = 1;
        for(int i = MAX_LEN - 1; i >= 0; i--) {
            t2 = _addcarry_base(0, t2, ~(c[i]), c[i]);
        }
    }
    return (t1 == 0) ? 1 : 0;
}

/*
    a = a - b
    返回是否溢出
*/
template <typename UINT_T, int MAX_LEN>
inline int minus_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    //    assert(a.size() == b.size());
    UINT8 t1 = 1;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], ~(b[i]), a[i]);
    }
    if(t1 == 0) {
        UINT8 t2 = 1;
        for(int i = MAX_LEN - 1; i >= 0; i--) {
            t2 = _addcarry_base(0, t2, ~(a[i]), a[i]);
        }
    }
    return (t1 == 0) ? 1 : 0;
}

/*
    c = a + b
    返回是否溢出
*/
template <typename UINT_T, int MAX_LEN>
inline int
add(const FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b, FixPointData<UINT_T, MAX_LEN>& c)
{
    //    assert(a.size() == b.size() && a.size() == c.size());
    UINT8 t1 = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], b[i], c[i]);
    }
    // 4 + 2 = 6,  t1 = 0
    // 4 + 6 = 10 = 2, t1 = 1
    // 4 + 4 = 8 = 0, t1 = 1
    return (t1 == 1) ? 1 : 0;
}

/*
    a = a + b
    返回是否溢出
*/
template <typename UINT_T, int MAX_LEN>
inline int add_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    //    assert(a.size() == b.size());
    UINT8 t1 = 0;
    for(int i = MAX_LEN - 1; i >= 0; i--) {
        t1 = _addcarry_base(t1, a[i], b[i], a[i]);
    }
    return (t1 == 1) ? 1 : 0;
}

/*
    a = a + b
    返回是否溢出
*/
template <typename UINT_T, int MAX_LEN> inline UINT_T add_eq(FixPointData<UINT_T, MAX_LEN>& a, const UINT_T b)
{
    UINT_T t1 = b;
    for(size_t i = 0; i < MAX_LEN; ++i) {
        t1 = _addcarry_base(0, a[i], t1, a[i]);
        if(t1 == 0) {
            break;
        }
    }
    return t1; //表示溢出
}

template <typename UINT_T, int MAX_LEN>
inline void KaratsubaMult_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b)
{
    //    assert(MAX_LEN % 2 == 0);
    UINT_T c[MAX_LEN * 2] = { 0 };
    MultBase::Multiplication<UINT_T, MAX_LEN>(c, (UINT_T*)a.data(), (UINT_T*)b.data());
    //    MultBase::Multiplication<UINT_T, MAX_LEN>(c, (UINT_T*)a.Data, (UINT_T*)b.Data);
    //    UINT8 carry = (c[MAX_LEN + 1] >> (sizeof(UINT_T) * 8 - 1));
    //    for(int i = MAX_LEN; i > 0 && carry; i--) {
    //        carry = _addcarry_base(carry, 0, c[i], c[i]);
    //    }
    memcpy((void*)(a.data()), (void*)(c + 1), MAX_LEN * sizeof(UINT_T));
}

template <typename T> inline void println(const T& a, const char* str)
{
    std::cout << std::endl << str;
    for(size_t i = 0; i < a.size(); ++i) {
        std::cout << "  " << a[i] << "  ";
    }
    printf(" \n");
}

template <typename UINT_T, int MAX_LEN> inline int lzcnt(const FixPointData<UINT_T, MAX_LEN>& a)
{
    return MultBase::lzcnt<UINT_T, MAX_LEN>(a.data());
}

template <typename UINT_T, int MAX_LEN> double ToDouble(const FixPointData<UINT_T, MAX_LEN>& a)
{
    return a[0] + a[1] / pow(2.0, 8 * sizeof(UINT_T));
}

template <typename UINT_T, int MAX_LEN> inline int iszero(const FixPointData<UINT_T, MAX_LEN>& a)
{
    for(size_t i = 0; i < MAX_LEN; ++i) {
        if(a[i] != 0) {
            return 0;
        }
    }
    return 1;
}

template <typename UINT_T, int MAX_LEN> int format(FixPointData<UINT_T, MAX_LEN>& a)
{
    int index = 0;
    if(a[0] != 0) {
        while(a[0] > 1) {
            index++;
            right_shift(a, 1);
        }
    } else {
        if(iszero(a)) {
            index = 0;
        } else {
            while(a[0] == 0) {
                index--;
                left_shift(a, 1);
            }
        }
    }
    return index;
}

/*
    a = a * b
    返回 void
*/
template <typename UINT_T, int MAX_LEN>
inline void FloatPoint_multi_eq(FixPointData<UINT_T, MAX_LEN>& a, const FixPointData<UINT_T, MAX_LEN>& b, int b_size)
{
    assert(b_size <= b.size());
    assert(b_size <= a.size());
    int a_idx_end = MAX_LEN - 1;
    if(b_size > MAX_LEN) {
        b_size = MAX_LEN;
    }
    int b_idx_end = b_size - 1;
    FixPointData<UINT_T, MAX_LEN> p = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    FixPointData<UINT_T, MAX_LEN> d = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    UINT_T hi;
    UINT_T dst;
    UINT_T bi;
    for(int i = b_idx_end; i >= 0; i--) {
        bi = b[i];
        for(int j = a_idx_end - i; j >= 0; j--) {
            dst = _mulx_base(a[j], bi, hi);
            int ij = i + j; // 0 -> a_idx_end

            if(ij > 0) {
                d[ij] += _addcarry_base(0, p[ij], dst, p[ij]);
                --ij;
                d[ij] += _addcarry_base(0, p[ij], hi, p[ij]);
            } else {
                _addcarry_base(0, p[ij], dst, p[ij]);
            }
        }
    }
    UINT_T d_MAX_LEN = 0; // d[MAX_LEN]
    UINT_T p_MAX_LEN = 0; // p[MAX_LEN]
    // i + j = MAX_LEN
    for(int i = 0; i < b_size; ++i) {
        dst = _mulx_base(a[MAX_LEN - i], b[i], hi);
        d_MAX_LEN += _addcarry_base(0, p_MAX_LEN, dst, p_MAX_LEN);
        d[a_idx_end] += _addcarry_base(0, p[a_idx_end], hi, p[a_idx_end]);
    }
    UINT8 carry = 0;
    if(p_MAX_LEN >= (UINT_T(1) << (8 * sizeof(UINT_T) - 1))) {
        carry = 1;
    }
    carry = _addcarry_base(carry, p[a_idx_end], d_MAX_LEN, a[a_idx_end]);
    for(int n = a_idx_end - 1; n >= 0; --n) {
        carry = _addcarry_base(carry, p[n], d[n + 1], a[n]);
    }
}
} // namespace FixPointOperator
#endif // FixPointOperator_H
