#ifndef DecimalConver_H
#define DecimalConver_H

template <typename UINT_T, size_t MAX_LEN> inline Decimal<UINT_T, MAX_LEN>& Decimal<UINT_T, MAX_LEN>::DivBy10()
{
    FixPointOperator::shift_divide_eq_By_10<UINT_T, MAX_LEN>(this->mData);
    this->mIndex = this->mIndex - UINT_BIT_SIZE;
    this->Format();
    return *this;
}

template <typename UINT_T, size_t MAX_LEN> int Decimal<UINT_T, MAX_LEN>::FormString(const char* nstr)
{
    int sgn = 0;
    if(nstr[0] == '+') {
        nstr++;
    } else if(nstr[0] == '-') {
        sgn = 1;
        nstr++;
    } else {
        sgn = 0;
    }
    int point_found = -1;
    std::string str1;
    str1.reserve(MAX_DIGITS_LEN);
    std::string str2;
    str2.reserve(MAX_DIGITS_LEN);

    for(int i = 0; nstr[i] != '\0'; ++i) {
        char ch = nstr[i];
        if(ch == ' ') {
            continue;
        }
        if(ch == '.') {
            point_found = i;
            continue;
        }
        if(point_found == -1) {
            str1.push_back(ch);
        } else {
            str2.push_back(ch);
        }
    }
    this->setNumber(0);
    this->mIndex = 0;
    //        DEBUG_COUT(nstr);
    //        DEBUG_COUT(str1);
    //        DEBUG_COUT(str2);
    for(int i = 0; i < (int)str1.length(); ++i) {
        char num = str1[i] - '0';
        (*this) *= 10;
        //        this->multi_eq_by(10);
        if(num != 0) {
            (*this) += Decimal<UINT_T, MAX_LEN>(num);
        }
    }
    //        DEBUG_COUT(this->ToDouble());
    if(point_found != -1) {
        if(str1.length() >= Decimal<UINT_T, MAX_LEN>::MAX_DIGITS_LEN) {
            str2 = str2.substr(0, 1);
        } else {
            if(Decimal<UINT_T, MAX_LEN>::MAX_DIGITS_LEN - str1.length() + 1 < str2.length()) {
                str2 = str2.substr(0, Decimal<UINT_T, MAX_LEN>::MAX_DIGITS_LEN - str1.length() + 1);
            }
        }
        Decimal<UINT_T, MAX_LEN> Fraction = Decimal<UINT_T, MAX_LEN>(0);
        Decimal<UINT_T, MAX_LEN> _power10 = Decimal<UINT_T, MAX_LEN>(1);
        for(int i = 0; i < (int)str2.length(); ++i) {
            //                        DEBUG_COUT(i);
            //                        DEBUG_COUT(_power10.ToDouble());
            //            _power10.println("10:", 100);
            char num = str2[i] - '0';
            //            _power10 /= 10;
            _power10.DivBy10();
            if(num != 0) {
                //    Fraction += (CTX::Power10(-i - 1) * num);
                Fraction += (_power10 * num);
            }
        }
        *this += Fraction;
        //                DEBUG_COUT(Fraction.ToDouble());
    }
    this->Format();
    this->mSgn = sgn;
    return 0;
}

// SFC: significant_figure_count
template <typename UINT_T, size_t MAX_LEN> std::string Decimal<UINT_T, MAX_LEN>::ToString(size_t SFC) const
{
    if(this->mIndex < 0) // Fraction Part
    {
        std::string B = this->FractionalPartString(-1, SFC);
        //        DEBUG_COUT(B);
        if(B[0] != '0') {
            B = "0." + B;
            if(this->mSgn == 1) {
                B.insert(0, 1, '-');
            }
            return B;
        }
        if(B.size() == 1 && B[0] == '0') {
            return B;
        }
        int index = -1;
        while(B.front() == '0') {
            B.erase(B.begin());
            index--;
        }
        //        DEBUG_COUT(B);
        if(B.empty()) {
            return "0";
        } else {
            while(B.length() > SFC) {
                B.pop_back();
            }
            //            DEBUG_COUT(B);
            if(B.length() > 1) {
                B.insert(1, 1, '.');
            }
            if(index != -1) {
                B = B + "E" + std::to_string(index);
            }
            if(this->mSgn == 1) {
                B.insert(0, 1, '-');
            }
            return B;
        }
    }
    if(this->mIndex == 0) {
        if(this->isZero()) {
            return "0";
        }
    }
    std::string A = this->IntegerPartString();
    if(SFC <= A.length()) {
        if(this->mSgn == 1) {
            A = "-" + A;
        }
        return A;
    }

    std::string B = this->FractionalPartString(SFC - A.length(), SFC);
    //    DEBUG_COUT(B);
    //    DEBUG_COUT(A);
    std::string C;
    C.reserve(MAX_DIGITS_LEN);
    if(B.length() == 1 && B[0] == '0') // B = 0
    {
        if(this->mSgn == 1) {
            C = "-" + A;
        } else {
            C = A;
        }
        return C;
    } else // C = A + B
    {
        C = A + "." + B;
    }
    //    DEBUG_COUT(A);
    //        DEBUG_COUT(B);
    //                std::cout << "LINE" << __LINE__ << ":" << C << std::endl;
    if(C.find('.') != std::string::npos) {
        while(C.length() > SFC + 1) {
            C.pop_back();
        }
        //        while(C.back() == '0')
        //        {
        //            C.pop_back();
        //        }
        //        std::cout << "LINE" << __LINE__ << ":" << C << std::endl;
        if(C.back() == '.') {
            C.pop_back();
        }
        //        std::cout << "LINE" << __LINE__ << ":"<< C << std::endl;
    }
    if(this->mSgn == 1) {
        C = "-" + C;
    }
    return C;
}

//            char nchar = '0';
//            if(nchar >= '8') {
//                int carry = 1;
//                for(auto it = C.rbegin(); it != C.rend(); it++) {
//                    if((*it) != '.') {
//                        (*it)++;
//                        if((*it) > '9') {
//                            (*it) = '0';
//                        } else {
//                            carry = 0;
//                            break;
//                        }
//                    }
//                }
//                if(carry == 1) {
//                    C = "1" + C;
//                }
//            }
//            std::cout << "LINE" << __LINE__ << ":" << C << std::endl;

// lenth: length of str
// SFC : 有效数字
template <typename UINT_T, size_t MAX_LEN>
std::string Decimal<UINT_T, MAX_LEN>::FractionalPartString(int lenth, int SFC) const
{
    if(lenth <= 0) {
        lenth = 0x7FFFFFF;
    }
    Decimal<UINT_T, MAX_LEN> b = Decimal<UINT_T, MAX_LEN>::abs(*this).GetFraction();
    //    DEBUG_COUT(b.mData[0]);
    //    DEBUG_COUT(b.mData[1]);
    //    DEBUG_COUT(b.ToDouble());
    std::string B;
    B.reserve(MAX_DIGITS_LEN);

    int idx;
    if constexpr(std::is_same<UINT_T, UINT16>::value) {
        idx = 2;
    } else {
        idx = 4;
    }
    int max_iter = (lenth / idx + 1) * idx;
    //    DEBUG_COUT(max_iter);
    //    max_iter = min(MAX_DIGITS_LEN, max_iter);
    //    DEBUG_COUT(max_iter);
    bool SF_flag = 0;
    int SF_count = 0;
    UINT_T pow10_idx = std::pow(10, idx);
    for(int i = 0; i < max_iter; i += idx) {
        //        b *= CTX::Power10(idx);
        b *= pow10_idx;

        char tt[8];
        unsigned int tmp = (unsigned int)b.ToUINT();
        if constexpr(std::is_same<UINT_T, UINT16>::value) {
            sprintf_s(tt, "%02u", tmp);
        } else {
            sprintf_s(tt, "%04u", tmp);
        }
        B = B + tt;
        b = b.GetFraction();

        if(tmp == 0) {
            SF_flag = (SF_count > 0);
        } else {
            SF_flag = 1;
        }
        if(SF_flag) {
            SF_count += idx;
        }
        if(SF_count > SFC) {
            break;
        }

        //        DEBUG_COUT(SF_flag);
        //        DEBUG_COUT(SF_count);
        //        DEBUG_COUT(tmp);
    }
    //    DEBUG_COUT(B);
    if(B.empty()) {
        B = "0";
        return B;
    } else {
        while(!B.empty() && B.back() == '0') {
            B.pop_back();
        }
    }
    if(B.empty()) {
        B = "0";
    }
    return B;
}

template <typename UINT_T, size_t MAX_LEN> std::string Decimal<UINT_T, MAX_LEN>::IntegerPartString() const
{
    assert(this->mIndex >= 0);
    std::string A;
    A.reserve(MAX_DIGITS_LEN);

    FixPointData<UINT_T, MAX_LEN> Intpart = Decimal<UINT_T, MAX_LEN>::abs(*this).IntegerPart();
    for(int i = 0; i < MAX_DIGITS_LEN; ++i) {
        //        char c = '0' + (char)FixPointOperator::divide_eq<UINT_T, MAX_LEN>(Intpart, 10);
        char c = '0' + FixPointOperator::divide_eq_By_10<UINT_T, MAX_LEN>(Intpart);
        A.push_back(c);
        if(FixPointOperator::lzcnt(Intpart) >= Data_BIT_SIZE) {
            break;
        }
    }
    if(A.empty()) {
        A = "0";
    } else {
        std::reverse(A.begin(), A.end());
    }
    //        DEBUG_COUT(A);
    //    bool carry = 0;
    //    for(auto it = A.rbegin(); it != A.rend(); ++it) {
    //        if(carry == 1) { ++(*it); }
    //        if((*it) > '9') {
    //            (*it) -= 10;
    //            carry = 1;
    //        } else {
    //            carry = 0;
    //        }
    //    }
    //    if(carry == 1) {
    //        A.insert(0, 1, '1');
    //    } else {
    //        while(A.front() == '0') {
    //            A.erase(A.begin());
    //        }
    //    }
    return A;
}

template <typename UINT_T, size_t MAX_LEN> Decimal<UINT_T, MAX_LEN> Decimal<UINT_T, MAX_LEN>::floor() const
{
    Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(0);
    if(this->mIndex < 0) {
        return A;
    } else if(this->mIndex == 0) {
        A.mData[0] = this->mData[0];
        return A;
    } else if(this->mIndex >= int((MAX_LEN - 1) * UINT_BIT_SIZE)) {
        return *this;
    } else {
        size_t count = (mIndex + 0) / UINT_BIT_SIZE + 1;
        short left = mIndex % UINT_BIT_SIZE;
        for(size_t i = 0; i < count; ++i) {
            A.mData[i] = this->mData[i];
        }
        if(left != 0 && count < MAX_LEN) {
            UINT_T end = this->mData[count];
            end = (end >> (UINT_BIT_SIZE - left)) << (UINT_BIT_SIZE - left);
            A.mData[count] = end;
        }
        A.mSgn = this->mSgn;
        A.mIndex = this->mIndex;
        A.Format();
        return A;
    }
}

template <typename UINT_T, size_t MAX_LEN> Decimal<UINT_T, MAX_LEN> Decimal<UINT_T, MAX_LEN>::round() const
{
    if(this->mSgn == 1) {
        Decimal<UINT_T, MAX_LEN> a = abs(*this);
        a = a.round();
        a.SetNegative();
        return a;
    }
    Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(0);
    if(this->mIndex < -1) {
        return A;
    } else if(this->mIndex == -1) {
        A.mData[0] = this->mData[0];
        return A;
    } else {
        A.mData[0] = 1;
        A.mIndex = -1;
        A += *this;
        return A.floor();
    }
}

template <typename UINT_T, size_t MAX_LEN> Decimal<UINT_T, MAX_LEN> Decimal<UINT_T, MAX_LEN>::GetFraction() const
{
    if(this->mIndex < 0) {
        return *this;
    } else if(this->mIndex == 0) {
        Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
        A.mData[0] = 0;
        A.Format();
        return A;
    } else {
        Decimal<UINT_T, MAX_LEN> A = Decimal<UINT_T, MAX_LEN>(*this);
        // mIndex = 0 => 32bit
        // mIndex = 1 => 33bit
        //        DecimalData GUINT = DecimalData(this->mData);
        int count = (mIndex) / UINT_BIT_SIZE + 1;
        for(int i = 0; i < count; ++i) {
            A.mData[i] = 0;
        }
        int ls = mIndex % UINT_BIT_SIZE;
        if(ls != 0) {
            //            std::cout << "L" << __LINE__ << " : " << ls << std::endl;
            //            std::cout << "L" << __LINE__ << " : " << A.mData[count] << std::endl;
            A.mData[count] = ((A.mData[count]) << ls);
            A.mData[count] = ((A.mData[count]) >> ls);
        }
        A.Format();
        return A;
    }
}

template <typename UINT_T, size_t MAX_LEN> UINT_T Decimal<UINT_T, MAX_LEN>::ToUINT() const
{
    //    if(this->mIndex >= UINT_BIT_SIZE) {
    //        std::cout << "L358:"<<this->mIndex << std::endl;
    //    }
    assert(this->mIndex < UINT_BIT_SIZE);
    if(this->mIndex < 0) {
        return 0;
    } else if(this->mIndex == 0) {
        UINT_T t = this->mData[0];
        return t;
    } else if(this->mIndex < UINT_BIT_SIZE) {
        int rs = this->mIndex;
        UINT_T t = this->mData[0];
        t = (t << rs) + ((this->mData[1]) >> (UINT_BIT_SIZE - rs));
        return t;
    } else {
        return 0;
    }
}

template <typename UINT_T, size_t MAX_LEN> Decimal<UINT_T, MAX_LEN>::operator double() const
{
    if constexpr(MAX_LEN == 2 || std::is_same<UINT_T, UINT64>::value) {
        double t = mData[0] + (mData[1]) * std::pow(2.0, -UINT_BIT_SIZE);
        t *= std::pow(2.0, mIndex);
        if(mSgn) {
            t = -t;
        }
        return t;
    } else {
        double t =
            mData[0] + (mData[1]) * std::pow(2.0, -UINT_BIT_SIZE) + (mData[2]) * std::pow(2.0, -UINT_BIT_SIZE * 2);
        t *= std::pow(2.0, mIndex);
        if(mSgn) {
            t = -t;
        }
        return t;
    }
}

#endif // DecimalConver_H
