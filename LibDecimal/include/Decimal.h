#ifndef Decimal_H
#define Decimal_H

/**
 * @class Decimal
 * @author Prean
 * @date 02/04/2019
 * @file Decimal.h
 * @brief 浮点小数，假设 FixPointData[0] = 1,即最大值为1.999999……
 * 浮点小数, 1.2 = {1,2,0,0}
 * 固定长度
 * 因为零位大，采用大端表示
 */

template <typename UINT_T, size_t MAX_LEN> struct Decimal {
    //    friend class CTX_MAP<Decimal>;
    //    friend class CTX_VECTOR<Decimal>;
    //    typedef CTX_VECTOR<Decimal> CTX;
    static_assert(std::is_arithmetic<UINT_T>::value != 0, "UINT_T Must be arithmetic type");
    static_assert(std::is_integral<UINT_T>::value != 0, "UINT_T Must be integral type");
    static_assert(MAX_LEN > 1, "MAX_LEN Must > 1");

public:
    typedef UINT_T UINT;
    explicit Decimal()
        : mData(MAX_LEN)
        , mIndex(0)
        , mSgn(0){};
    Decimal(const Decimal& a)
        : mData(a.mData)
        , mIndex(a.mIndex)
        , mSgn(a.mSgn)
    {
    }
    explicit Decimal(const FixPointData<UINT_T, MAX_LEN>& Adata)
        : mData(Adata)
        , mIndex(0)
        , mSgn(0)
    {
    }
    explicit Decimal(const char* nstr)
        : mData(MAX_LEN)
        , mIndex(0)
        , mSgn(0)
    {
        this->FormString(nstr);
    }
    explicit Decimal(const UINT_T num)
        : mData(MAX_LEN)
        , mIndex(0)
        , mSgn(0)
    {
        mData[0] = num;
        this->Format();
    }
    explicit Decimal(const int num)
        : mData(MAX_LEN)
        , mIndex(0)
        , mSgn(0)
    {
        if constexpr(sizeof(UINT_T) < sizeof(int)) {
            //        if((std::abs(num) >> UINT_BIT_SIZE) != 0) { DEBUG_COUT(num); }
            assert((std::abs(num) >> UINT_BIT_SIZE) == 0);
        }
        mData[0] = std::abs(num);
        this->Format();
        if(num < 0) {
            this->mSgn = 1;
        }
    }

    Decimal& operator=(const Decimal& a)
    {
        if(this != &a) {
            this->mIndex = a.mIndex;
            this->mSgn = a.mSgn;
            this->mData = a.mData;
        }
        return *this;
    }

    inline Decimal operator+(const Decimal& b) const
    {
        if(this->mSgn == b.mSgn) {
            return _add(*this, b);
        } else {
            return _minus(*this, b);
        }
    }
    inline Decimal& operator+=(const Decimal& b)
    {
        if(this->mSgn == b.mSgn) {
            _add_eq(*this, b);
        } else {
            _minus_eq(*this, b);
        }
        return *this;
    }

    inline Decimal operator-(const Decimal& b) const
    {
        if(this->mSgn != b.mSgn) {
            return _add(*this, b);
        } else {
            return _minus(*this, b);
        }
    }
    inline Decimal& operator-=(const Decimal& b)
    {
        if(this->mSgn != b.mSgn) {
            _add_eq(*this, b);
        } else {
            _minus_eq(*this, b);
        }
        return *this;
    }

    inline Decimal operator*(const Decimal& b) const
    {
        Decimal A = Decimal(*this);
        A *= b;
        return A;
    }
    inline Decimal operator*(int num) const
    {
        Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
        A *= num;
        return A;
    }
    inline Decimal operator*(UINT_T num) const
    {
        Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
        A *= num;
        return A;
    }
    inline Decimal& operator*=(const Decimal& b)
    {
        //        FixPointOperator::multi_eq<UINT_T, MAX_LEN>(this->mData, b.mData);
        FixPointOperator::KaratsubaMult_eq<UINT_T, MAX_LEN>(this->mData, b.mData);
        this->mIndex += b.mIndex;
        this->Format();
        this->mSgn = (this->mSgn == b.mSgn) ? 0 : 1;
        return *this;
    }

    inline Decimal& operator*=(int num)
    {
        unsigned int n = std::abs(num);
        if constexpr(sizeof(UINT_T) < sizeof(int)) {
            //            if((n >> UINT_BIT_SIZE) != 0) { DEBUG_COUT(num); }
            assert((n >> UINT_BIT_SIZE) == 0);
        }

        *this *= (UINT_T)n;
        if(num < 0) {
            this->mSgn = (this->mSgn) ? 0 : 1;
        }
        return *this;
    }

    inline Decimal& operator*=(UINT_T n)
    {
        if(n == 0) {
            this->setNumber(0);
            this->mIndex = 0;
            this->mSgn = 0;
            return *this;
        }
        //        if(n == 1) { return *this; }
        //        else if(n == 2) {
        //            //            DEBUG_COUT(num);
        //            this->mIndex = this->mIndex + 1;
        //            return *this;
        //        }
        //        else
        if((n & (n - 1)) == 0) {
            this->mIndex = this->mIndex + (int)(nzcnt_base(n) - 1);
            return *this;
        } else {
            FixPointOperator::multi_eq<UINT_T, MAX_LEN>(this->mData, n);
            this->Format();
            return *this;
        }
    }

    inline Decimal ToReciprocal() const
    {
        if(this->mData[0] == 0) {
            return Decimal(0);
        }
        Decimal B = Decimal();
        //        FixPointOperator::FloatPointReciprocal<UINT_T, MAX_LEN>(this->mData, B.mData);
        //        FixPointOperator::Base_Reciprocal<UINT_T, MAX_LEN>(this->mData, B.mData);
        //        FixPointOperator::KaratsubaReciprocal<UINT_T, MAX_LEN>(this->mData, B.mData);
        //        Karatsuba::Reciprocal<UINT_T, MAX_LEN>(this->mData.data(), B.mData.data());
        FixPointOperator::FloatPointReciprocal2<UINT_T, MAX_LEN>(this->mData.data(), B.mData.data());
        B.mSgn = this->mSgn;
        //        DEBUG_COUT(B.ToDouble());
        B.mIndex = -this->mIndex;
        B.Format();
#ifndef NDEBUG
        Decimal err = Decimal(1) - B * (*this);
        if(!err.eps(4)) {
            DEBUG_COUT(double(B));
            DEBUG_COUT(double(*this));
            DEBUG_COUT(err.ToString(10));
            DEBUG_COUT((*this).mData[1]);
        }
#endif
        return B;
    }

    inline Decimal operator/(const Decimal& b) const
    {
        return (*this) * b.ToReciprocal();
    }
    inline Decimal& operator/=(const Decimal& b)
    {
        (*this) *= b.ToReciprocal();
        return *this;
    }
    inline Decimal operator/(UINT_T num) const
    {
        return Decimal(*this) /= num;
    }

    inline Decimal& operator/=(UINT_T num)
    {
        if(num == 0) {
            assert(num != 0 && "Divide by zero");
            memset(this->mData.data(), 0xFF, MAX_LEN * sizeof(UINT_T));
            this->mIndex = 0;
            return *this;
        } else if(num == 1) {
            return *this;
        } else if(num == 2) {
            this->mIndex = this->mIndex - 1;
            return *this;
        } else if((num & (num - 1)) == 0) {
            this->mIndex = this->mIndex - (int)(nzcnt_base(num) - 1);
            return *this;
        } else {
            FixPointOperator::shift_divide_eq<UINT_T, MAX_LEN>(this->mData, num);
            this->mIndex = this->mIndex - UINT_BIT_SIZE;
            this->Format();
            return *this;
        }
    }

    inline Decimal& operator/=(int num)
    {
        unsigned int n = std::abs(num);
        if constexpr(sizeof(UINT_T) < sizeof(int)) {
            //            if((n >> UINT_BIT_SIZE) != 0) { DEBUG_COUT(num); }
            assert((n >> UINT_BIT_SIZE) == 0);
        }

        *this /= (UINT_T)n;
        if(num < 0) {
            this->mSgn = (this->mSgn) ? 0 : 1;
        }
        return *this;
    }

    inline Decimal& DivBy10();
    /* 0x1F = 31 / 0x05 = 6 = 0x06
     */

    inline explicit operator double() const;
    inline Decimal operator-() const
    {
        Decimal A = Decimal(*this);
        A.mSgn = (A.mSgn) ? 0 : 1;
        return A;
    }
    int FormString(const char* nstr);
    //    std::string ToDecimalString(int lenth = 8);
    std::string FractionalPartString(int lenth, int SFC) const;
    std::string IntegerPartString() const;
    std::string ToString(size_t lenth = MAX_DIGITS_LEN) const;

    friend inline std::ostream& operator<<(std::ostream& os, Decimal& d)
    {
        os << d.ToString(50);
        return os;
    }

    FixPointData<UINT_T, MAX_LEN> IntegerPart() const
    {
        if(this->mIndex < 0) {
            return FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
        } else {
            int rs = SF_BIT_SIZE - this->mIndex - 1;
            if(rs >= 0) {
                FixPointData<UINT_T, MAX_LEN> A = FixPointData<UINT_T, MAX_LEN>(this->mData);
                FixPointOperator::right_shift<UINT_T, MAX_LEN>(A, rs);
                return A;
            } else {
                printf_s("\n  In File:%s, Function: %s(), Line: %d \n Number is too great!\n\n", __FILE__, __FUNCTION__,
                    __LINE__);
                return FixPointData<UINT_T, MAX_LEN>(MAX_LEN);
            }
        }
    }
    UINT_T ToUINT() const;
    //    double ToDouble() const;
    Decimal floor() const;
    Decimal round() const;
    static Decimal abs(const Decimal b)
    {
        Decimal a = b;
        a.mSgn = 0;
        return a;
    }
    Decimal GetFraction() const;
    Decimal frexpf() const
    {
        Decimal A = *this;
        A.mIndex = 0;
        return A;
    }
    inline void println(const char* str = nullptr, int numcount = 0) const
    {
        if(str != nullptr) {
            std::cout << str;
        }
        if(numcount <= 0) {
            std::cout << this->ToString() << std::endl;
        } else {
            std::cout << this->ToString(numcount) << std::endl;
        }
    }
    inline Decimal TEST_ToString(const char* str = nullptr, int numcount = 0)
    {
        Decimal Err = Decimal(ToString().c_str()) - (*this);
        Err.mSgn = 0;
        //        DEBUG_COUT(Err.mIndex);
        //        Err.floor().println("a floor:", 20);
        //        Err.GetFraction().println("a floor:", 20);
        if(str != nullptr) {
            std::cout << "String input and output error of " << str << " : ";
        } else {
            printf("String input and output error: ");
        }
        if(numcount <= 0) {
            std::cout << Err.ToString() << std::endl;
        } else {
            std::cout << Err.ToString(numcount) << std::endl;
        }
        return Err;
    }
    inline bool isPositive() const
    {
        return this->mSgn == 0;
    }
    inline bool isZero() const
    {
        return FixPointOperator::iszero<UINT_T, MAX_LEN>(mData);
    }
    inline bool isNegative() const
    {
        return this->mSgn != 0;
    }
    inline void SetPositive()
    {
        this->mSgn = 0;
    }
    inline void SetNegative()
    {
        this->mSgn = 1;
    }
    inline void ChangeSgn()
    {
        this->mSgn = (mSgn) ? 0 : 1;
    }
    inline int GetIndex() const
    {
        return this->mIndex;
    }
    inline int AddIndex(int m)
    {
        return (this->mIndex += m);
    }
    inline void SetIndex(int idx)
    {
        mIndex = idx;
    }
    inline bool eps(int index = 1) const
    {
        return (mIndex + Decimal::SF_BIT_SIZE < index || mData[0] == 0);
    }

    constexpr static inline int UINT_BIT_SIZE = (8 * sizeof(UINT_T));

    constexpr static inline int Data_BIT_SIZE = MAX_LEN * UINT_BIT_SIZE;
    constexpr static inline int SF_BIT_SIZE = Decimal::Data_BIT_SIZE - UINT_BIT_SIZE + 1;
    constexpr static inline UINT_T UINT_T_MAX = ~((UINT_T)0);

protected:
    FixPointData<UINT_T, MAX_LEN> mData;
    int mIndex;
    int mSgn;

    // constexpr static inline int Zero_Index = UINT_BIT_SIZE - 1;
    constexpr static inline int MAX_RECIPROCAL_ITER = 2 + ilog2<MAX_LEN>::value;
    constexpr static inline int MAX_DIGITS_LEN = 1 + (int)(SF_BIT_SIZE * 0.301029996);
    // 32   => 8
    // 128  => 9
    // 256  => 10
    // 512  => 11
    // 1024 => 12

    void setNumber(UINT_T n);
    //    Decimal multi_by(UINT_T num) const;
    //    void multi_eq_by(UINT_T num);
    int Format();
    static Decimal _add(const Decimal& a, const Decimal& b);
    static Decimal _minus(const Decimal& a, const Decimal& b);
    static void _add_eq(Decimal& a, const Decimal& b);
    static void _minus_eq(Decimal& a, const Decimal& b);
};

template <typename UINT_T, size_t MAX_LEN> void Decimal<UINT_T, MAX_LEN>::setNumber(UINT_T n)
{
    mData[0] = n;
    for(size_t i = 1; i < MAX_LEN; ++i) {
        mData[i] = 0;
    }
}

// template <typename UINT_T, int MAX_LEN>
// void Decimal<UINT_T, MAX_LEN>::multi_eq_by(UINT_T num)
//{
//    switch(num)
//    {
//        case 0:
//        {
//            this->setNumber(0);
//            break;
//        }
//        case 10:
//        {
//            Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
//            A.mIndex++;
//            this->mIndex += 3;
//            *this += A;
//            break;
//        }
//        case 1:
//        {
//            break;
//        }
//        case 2:
//        {
//            this->mIndex++;
//            //            A.Format();
//            break;
//        }
//        case 3:
//        {
//            Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
//            A.mIndex++;
//            *this += A;
//            break;
//        }
//        case 4:
//        {
//            this->mIndex += 2;
//            //            A.Format();
//            break;
//        }
//        case 5:
//        {
//            Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
//            A.mIndex += 2;
//            *this += A;
//            break;
//        }
//        case 6:
//        {
//            Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
//            A.mIndex++;
//            this->mIndex += 2;
//            *this += A;
//            break;
//        }
//        case 8:
//        {
//            this->mIndex += 3;
//            //            A.Format();
//            break;
//        }
//        case 9:
//        {
//            Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
//            A.mIndex++;
//            this->mIndex += 3;
//            *this += A;
//            break;
//        }
//        default:
//        {
//            FixPointOperator::multi_eq<UINT_T, MAX_LEN>(this->mData, num);
//            break;
//        }
//    }
//    return;
//}

template <typename UINT_T, size_t MAX_LEN> int Decimal<UINT_T, MAX_LEN>::Format()
{
    if(mData[0] == 1) {
        return 0;
    } else if(mData[0] > 1) {
        int index = nzcnt_base(mData[0]) - 1;
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(mData, index);
        this->mIndex += index;
        //        if (mData[0] != 1) DEBUG_COUT(mData[0]);
        return this->mIndex;
    } else {
        size_t j = 0;
        for(size_t i = 1; i < MAX_LEN; ++i) {
            if(this->mData[i] != 0) {
                j = i;
                break;
            }
        }
        if(j == 0) {
            // iszero
            this->mIndex = 0;
            return 0;
        } else {
            int ls = (j - 1) * UINT_BIT_SIZE + lzcnt_base(mData[j]) + 1;
            FixPointOperator::left_shift<UINT_T, MAX_LEN>(mData, ls);
            //        if (mData[0] != 1) DEBUG_COUT(index);
            this->mIndex -= ls;
            return this->mIndex;
        }
    }
}

/**
 * @brief c = a + b
 * @param a
 * @param b
 */
template <typename UINT_T, size_t MAX_LEN>
Decimal<UINT_T, MAX_LEN> Decimal<UINT_T, MAX_LEN>::_add(const Decimal<UINT_T, MAX_LEN>& a,
    const Decimal<UINT_T, MAX_LEN>& b)
{
    FixPointData<UINT_T, MAX_LEN> Adata = FixPointData<UINT_T, MAX_LEN>(a.mData);
    FixPointData<UINT_T, MAX_LEN> Bdata = FixPointData<UINT_T, MAX_LEN>(b.mData);

    if(a.mIndex < b.mIndex) {
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Adata, b.mIndex - a.mIndex);
    }
    if(a.mIndex > b.mIndex) {
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Bdata, a.mIndex - b.mIndex);
    }

    FixPointOperator::add_eq<UINT_T, MAX_LEN>(Adata, Bdata);

    Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(Adata);

    A.mIndex = max(a.mIndex, b.mIndex);
    A.Format();
    A.mSgn = a.mSgn;
    return A;
}

/**
 * @brief a = a + b
 * @param a
 * @param b
 */
template <typename UINT_T, size_t MAX_LEN>
void Decimal<UINT_T, MAX_LEN>::_add_eq(Decimal<UINT_T, MAX_LEN>& a, const Decimal<UINT_T, MAX_LEN>& b)
{
    if(a.mIndex > b.mIndex) {
        FixPointData<UINT_T, MAX_LEN> Bdata = FixPointData<UINT_T, MAX_LEN>(b.mData);
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Bdata, a.mIndex - b.mIndex);
        FixPointOperator::add_eq<UINT_T, MAX_LEN>(a.mData, Bdata);
        a.Format();
        return;
    }
    if(a.mIndex < b.mIndex) {
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(a.mData, b.mIndex - a.mIndex);
    }
    FixPointOperator::add_eq<UINT_T, MAX_LEN>(a.mData, b.mData);
    a.mIndex = b.mIndex;
    a.Format();
    return;
}

/**
 * @brief c = a - b
 * @param a
 * @param b
 */
template <typename UINT_T, size_t MAX_LEN>
Decimal<UINT_T, MAX_LEN> Decimal<UINT_T, MAX_LEN>::_minus(const Decimal<UINT_T, MAX_LEN>& a,
    const Decimal<UINT_T, MAX_LEN>& b)
{
    //        assert(a.mSgn == b.mSgn);
    Decimal<UINT_T, MAX_LEN> C;
    FixPointData<UINT_T, MAX_LEN> Adata = FixPointData<UINT_T, MAX_LEN>(a.mData);
    FixPointData<UINT_T, MAX_LEN> Bdata = FixPointData<UINT_T, MAX_LEN>(b.mData);
    if(a.mIndex > b.mIndex) {
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Bdata, a.mIndex - b.mIndex);
    }
    if(a.mIndex < b.mIndex) {
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Adata, b.mIndex - a.mIndex);
    }

    int t1 = FixPointOperator::minus<UINT_T, MAX_LEN>(Adata, Bdata, C.mData);
    if(t1 == 1) {
        C.mSgn = (a.mSgn == 0);
    } else {
        C.mSgn = a.mSgn;
    }

    C.mIndex = max(a.mIndex, b.mIndex);
    C.Format();
    return C;
}

/**
 * @brief a = a - b
 * @param a
 * @param b
 */

template <typename UINT_T, size_t MAX_LEN>
void Decimal<UINT_T, MAX_LEN>::_minus_eq(Decimal<UINT_T, MAX_LEN>& a, const Decimal<UINT_T, MAX_LEN>& b)
{
    if(a.mIndex > b.mIndex) {
        FixPointData<UINT_T, MAX_LEN> Bdata = FixPointData<UINT_T, MAX_LEN>(b.mData);
        FixPointOperator::right_shift<UINT_T, MAX_LEN>(Bdata, a.mIndex - b.mIndex);
        FixPointOperator::minus_eq<UINT_T, MAX_LEN>(a.mData, Bdata);
        a.Format();
        return;
    } else {
        if(a.mIndex < b.mIndex) {
            FixPointOperator::right_shift<UINT_T, MAX_LEN>(a.mData, b.mIndex - a.mIndex);
            a.mIndex = b.mIndex;
        }
        int t1 = FixPointOperator::minus_eq<UINT_T, MAX_LEN>(a.mData, b.mData);
        if(t1 == 1) {
            a.mSgn = (a.mSgn == 0);
        }
        a.Format();
        return;
    }
}

#endif // Decimal_H
