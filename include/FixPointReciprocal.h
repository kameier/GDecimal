#ifndef FixPointReciprocal_H
#define FixPointReciprocal_H

namespace FixPointOperator
{

template <typename UINT_T, int MAX_LEN> inline int DoWithDivZero(FixPointData<UINT_T, MAX_LEN>& A)
{
    for(int i = 0; i < MAX_LEN; ++i) {
        A[i] = ~((UINT_T)0);
    }
    return -1;
}

template <int MAX_LEN> inline UINT16 reciprocal_init_value(UINT16 a1, UINT16* A)
{
    memset((void*)A, 0, MAX_LEN * sizeof(UINT16));
    float t1 = pow(2.0, 16) / (1 + (a1 + 1) * pow(2.0, -16));
    A[1] = static_cast<UINT16>(t1);
    if(A[1] == 0) {
        A[1] = UINT16(1) << 15;
    }
    return A[1];
    //    A1 = 0xB100;
}
template <int MAX_LEN> inline UINT32 reciprocal_init_value(UINT32 a1, UINT32* A)
{
    memset((void*)(A), 0, MAX_LEN * sizeof(UINT32));
    double t1 = pow(2.0, 32) / (1 + (a1 + 1) * pow(2.0, -32));
    A[1] = (UINT32)(t1);
    if(A[1] == 0) {
        A[1] = UINT32(1) << 31;
    }
    return A[1];
}
template <int MAX_LEN> inline UINT64 reciprocal_init_value(UINT64 a1, UINT64* A)
{
    memset((void*)(A), 0, MAX_LEN * sizeof(UINT64));
    const int i = 16;
    double t1 = 1.0 + ((a1 >> i) + 1) * pow(2.0, i - 64);
    A[1] = (UINT64)(pow(2.0, 64) / t1);
    return A[1];
}

/*
    A = 1 / a
    返回A = 1/a
    A(n+1) = A(n) + A(n)(1 - a*A(n))
*/
template <typename UINT_T, int MAX_LEN>
int FloatPointReciprocal(const FixPointData<UINT_T, MAX_LEN>& a, FixPointData<UINT_T, MAX_LEN>& A)
{
    if(a[0] == 0) {
        assert(a[0] != 0 && "Divide by zero");
        return DoWithDivZero(A);
    }
    reciprocal_init_value<MAX_LEN>(a[1], A.data());
    FixPointData<UINT_T, MAX_LEN> AA = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    int n = 0;
    int lz_count_limit = MAX_LEN * 8 * (int)sizeof(UINT_T) - 1;
    int lz_count = 0;
    int a_size = 2;
    for(n = 0; n < lz_count_limit; n++) {
        AA = A;
        FloatPoint_multi_eq(AA, a, a_size);
        if(AA[0] == 0) {
            UINT8 t1 = 1;
            for(int i = MAX_LEN - 1; i > 0; i--) {
                t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
            }
            lz_count = lzcnt(AA);
            //            DEBUG_COUT(lz_count - zerocount);
            FloatPoint_multi_eq(AA, A, a_size);
            add_eq(A, AA);
        } else // if(AA[0] == 1)
        {
            AA[0] = 0;
            lz_count = lzcnt(AA);
            //            DEBUG_COUT(lz_count - lz_count_limit);
            FloatPoint_multi_eq(AA, A, a_size);
            minus_eq(A, AA);
        }
        if(lz_count >= lz_count_limit) {
            break;
        }
        if(a_size < MAX_LEN) {
            a_size *= 2;
        }
    }
    //    printf("FixPoint_reciprocal iter times is %d\n", n);
    return n;
}

/*
    A = 1 / a
    返回A = 1/a
    A(n+1) = A(n) + A(n)(1 - a*A(n))
*/

template <typename UINT_T, int MAX_LEN>
int KaratsubaReciprocal(const FixPointData<UINT_T, MAX_LEN>& a, FixPointData<UINT_T, MAX_LEN>& A)
{
    if(a[0] == 0) {
        assert(a[0] != 0 && "Divide by zero");
        return DoWithDivZero(A);
    }
    reciprocal_init_value<MAX_LEN>(a[1], A.data());
    FixPointData<UINT_T, MAX_LEN> AA = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    int n;
    int lz_count_limit = MAX_LEN * 8 * (int)sizeof(UINT_T) - 1;
    int lz_count = 0;
    for(n = 0; n < lz_count_limit; n++) {
        AA = A;
        KaratsubaMult_eq<UINT_T, MAX_LEN>(AA, a);
        //        multi_eq<UINT_T, MAX_LEN>(AA, a);
        if(AA[0] == 0) {
            UINT8 t1 = 1;
            for(int i = MAX_LEN - 1; i > 0; i--) {
                t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
            }
            lz_count = lzcnt(AA);
            KaratsubaMult_eq<UINT_T, MAX_LEN>(AA, A);
            //            multi_eq<UINT_T, MAX_LEN>(AA, A);
            add_eq(A, AA);
        } else // if(AA[0] == 1)
        {
            AA[0] = 0;
            lz_count = lzcnt(AA);
            KaratsubaMult_eq<UINT_T, MAX_LEN>(AA, A);
            //            multi_eq<UINT_T, MAX_LEN>(AA, A);
            minus_eq(A, AA);
        }
        //        println(A,"A");
        //        println(AA,"AA");
        if(lz_count >= lz_count_limit) {
            break;
        }
    }
    //    if(n > 9)
    //    {
    //        DEBUG_COUT(ToDouble(a));
    //        DEBUG_COUT(lz_count - zerocount);
    //        printf("FixPoint_reciprocal iter times is %d\n", n);
    //    }
    //            printf("FixPoint_reciprocal iter times is %d\n", n);
    return n;
}

template <typename UINT_T, int MAX_LEN> int FloatPointReciprocal2(const UINT_T* a, UINT_T* A)
{
    UINT_T B[MAX_LEN] = { 0 };
    MultBase::Reciprocal<UINT_T, MAX_LEN>(a, A);
    int n;
    int lz_count_limit = MAX_LEN * 8 * (int)sizeof(UINT_T) - 2;
    int lz_count = 0;
    for(n = 1; n < lz_count_limit; n++) {
        memcpy(B, A, MAX_LEN * sizeof(UINT_T));
        MultBase::Mult_eq<UINT_T, MAX_LEN>(B, a);
        if(B[0] == 0) {
            UINT8 t1 = 1;
            for(int i = MAX_LEN - 1; i > 0; i--) {
                t1 = _addcarry_base(t1, ~(B[i]), 0, B[i]);
            }
            lz_count = MultBase::lzcnt<UINT_T, MAX_LEN>(B);
            MultBase::Mult_eq<UINT_T, MAX_LEN>(B, A);
            MultBase::add_eq<UINT_T, MAX_LEN>(A, B);
        } else {
            B[0] = 0;
            lz_count = MultBase::lzcnt<UINT_T, MAX_LEN>(B);
            MultBase::Mult_eq<UINT_T, MAX_LEN>(B, A);
            MultBase::minus_eq<UINT_T, MAX_LEN>(A, B);
        }
        //        DEBUG_COUT(n);
        if(lz_count >= lz_count_limit) {
            break;
        }
    }
    //    DEBUG_COUT(n);
    return n;
}

/*
    A = 1 / a
    返回A = 1/a
    A(n+1) = A(n) + A(n)(1 - a*A(n))
*/

template <typename UINT_T, int MAX_LEN>
int Base_Reciprocal(const FixPointData<UINT_T, MAX_LEN>& a, FixPointData<UINT_T, MAX_LEN>& A)
{
    if(a[0] == 0) {
        assert(a[0] != 0 && "Divide by zero");
        return DoWithDivZero(A);
    }
    reciprocal_init_value<MAX_LEN>(a[1], A.data());
    FixPointData<UINT_T, MAX_LEN> AA = FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
    int n;
    int lz_count_limit = MAX_LEN * 8 * (int)sizeof(UINT_T) - 1;
    int lz_count = 0;
    for(n = 0; n < lz_count_limit; n++) {
        AA = A;
        multi_eq<UINT_T, MAX_LEN>(AA, a);
        if(AA[0] == 0) {
            UINT8 t1 = 1;
            for(int i = MAX_LEN - 1; i > 0; i--) {
                t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
            }
            lz_count = lzcnt(AA);
            multi_eq<UINT_T, MAX_LEN>(AA, A);
            add_eq(A, AA);
        } else // if(AA[0] == 1)
        {
            AA[0] = 0;
            lz_count = lzcnt(AA);
            multi_eq<UINT_T, MAX_LEN>(AA, A);
            minus_eq(A, AA);
        }
        if(lz_count >= lz_count_limit) {
            break;
        }
    }
    return n;
}
// template <typename UINT_T, int MAX_LEN>
// int reciprocal(const FixPointData<UINT_T, MAX_LEN>& a, FixPointData<UINT_T, MAX_LEN>& A, size_t max_iter)
//{
//    int MaxLen = a.size();
//    assert(a[0] != 0);
//    assert(a.size() == A.size());
//    if(a[0] == 0)
//    {
//        std::cout << "FixPoint::reciprocal LINE" << __LINE__ << ":" << a[0] << std::endl;
//        for(int i = 0; i < (int)A.size(); ++i)
//        {
//            A[i] = ~((UINT_T)0);
//        }
//        return -1;
//    }
//    for(int i = 0; i < (int)A.size(); ++i)
//    {
//        A[i] = 0;
//    }
//    reciprocal_init_value(a[1], A[1]);
//    FixPointData<UINT_T, MAX_LEN> AA = FixPointData<UINT_T, MAX_LEN>(MaxLen);
//    int n;
//    int zerocount = MaxLen * 8 * (int)sizeof(UINT_T) - 2;
//    for(n = 0; n < (int)max_iter; n++)
//    {
//        // A(n+1) = A(n) + A(n)(1 - a*A(n))
//        AA = A;
//        multi_eq(AA, a);
//        UINT8 t1 = 1;
//        for(int i = MaxLen - 1; i > 0; i--)
//        {
//            t1 = _addcarry_base(t1, ~(AA[i]), 0, AA[i]);
//        }
//        t1 = _addcarry_base(t1, ~(AA[0]), 1, AA[0]);
//        if(t1 == 0)
//        {
//            UINT8 t2 = 1;
//            for(int i = MaxLen - 1; i >= 0; i--)
//            {
//                t2 = _addcarry_base(0, t2, ~(AA[i]), AA[i]);
//            }
//        }
//        //        if(n == 0)
//        //        {
//        //            DEBUG_COUT(ToDouble(AA)); // 1 - a*A
//        //            DEBUG_COUT(ToDouble(a));
//        //            DEBUG_COUT(ToDouble(A));
//        //        }
//        int lz_count = lzcnt(AA);
//        multi_eq(AA, A);
//        add_eq(A, AA);
//        if(lz_count >= zerocount)
//        {
//            break;
//        }
//    }
//    //        printf("FixPoint_reciprocal iter times is %d\n", n);
//    return n;
//}

// template <typename UINT_T, int MAX_LEN>
// int reciprocal(const FixPointData<UINT_T, MAX_LEN>& a, FixPointData<UINT_T, MAX_LEN>& A, size_t max_iter)
//{
//    int MaxLen = a.size();
//    assert(a[0] != 0);
//    assert(a.size() == A.size());
//    if(a[0] == 0)
//    {
//        std::cout << "FixPoint::reciprocal LINE" << __LINE__ << ":" << a[0] << std::endl;
//        for(int i = 0; i < (int)A.size(); ++i)
//        {
//            A[i] = 0xFFFFFFFF;
//        }
//        return 0;
//    }
//    for(int i = 0; i < (int)A.size(); ++i)
//    {
//        A[i] = 0;
//    }
//    reciprocal_init_value(a[1], A[1]);
//    //    std::cout << std::endl << "L434 : " << t1 << std::endl;
//    //    std::cout << std::endl << "L436 : " << std::hex << A[1] << std::endl;
//    FixPointData<UINT_T, MAX_LEN> B = FixPointData<UINT_T, MAX_LEN>(MaxLen);
//    FixPointData<UINT_T, MAX_LEN> AA = B;
//    FixPointData<UINT_T, MAX_LEN> two = B;
//    two[0] = 2;
//    int n;
//    int zerocount = MaxLen * 8 * (int)sizeof(UINT_T) - 2;
//    for(n = 0; n < (int)max_iter; n++)
//    {
//        // A(n+1) = A(n)*(2 - a*A(n))
//
//        AA = A;
//        //        DEBUG_COUT(A[0] + A[1] / pow(2.0, 64));
//        multi(a, A, B);
//        //        DEBUG_COUT(B[0] + B[1] / pow(2.0, 64));
//        minused_eq(two, B);
//        //        DEBUG_COUT(B[0] + B[1] / pow(2.0, 64));
//        multi_eq(A, B);
//        //        DEBUG_COUT(A[0] + A[1] / pow(2.0, 64));
//        minused_eq(A, AA);
//        //        DEBUG_COUT(AA[0] + AA[1] / pow(2.0, 64));
//        if(lzcnt(AA) >= zerocount)
//        {
//            break;
//        }
//    }
//    printf("FixPoint_reciprocal iter times is %d\n", n);
//    return n;
//}
} // namespace FixPointOperator
#endif // FixPointReciprocal_H
