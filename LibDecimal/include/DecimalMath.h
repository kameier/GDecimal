#ifndef DecimalMath_H
#define DecimalMath_H

#include "DecimalMathConstant.h"

namespace DecimalMath
{
// Power函数
template <typename T> T Power(T base, int exponent)
{
    // base为0，浮点数判断不能直接==
    if(base.isZero()) {
        return T(0);
    }
    // exponent为0返回1
    if(exponent == 0) {
        return T(1);
    }
    //先求正数次幂的值
    T ans = T(1);

    int exponent_abs = std::abs(exponent);
    while(exponent_abs) {
        if((exponent_abs & 1) == 1) {
            ans *= base;
        }
        base *= base;
        exponent_abs >>= 1;
    }
    //判断是否是负数次幂
    if(exponent < 0) {
        ans = T(1) / ans;
    }
    return ans;
}

/**
 * @brief Square root of x
 * @param x
 * @return dest = sqrt(x)
 * dest = (dest + x/dest)/2
 * err  = (err)^2/(2*dest)
 */
inline decimal sqrt(const decimal& x)
{
    assert(x.isPositive());
    //    decimal dest = decimal(1);
    // try 14/9 = 1.555555555555556
    decimal dest = decimal(1593);
    dest.AddIndex(x.GetIndex() / 2 - 10);
    constexpr int n = 10 + ilog2<decimal::SF_BIT_SIZE>::value;
    decimal t = decimal(1);
    for(int i = 1; i < n && !t.eps(dest.GetIndex()); i++) {
        t = x / dest;
        dest = dest + t;
        dest /= 2;
        t -= dest;
        //        DEBUG_COUT(t.ToString(10));
    }
    //    DEBUG_COUT(dest.ToString(10));
    return dest;
}

/**
 * @brief Cube root of x
 * @param x
 * @return dest = cbrt(x)
 * dest = (dest*2 + x/dest^2)/3
 */

inline decimal cbrt(const decimal& x)
{
    decimal dest = decimal(1);
    dest.SetIndex(x.GetIndex() / 3);
    constexpr int n = 10 + ilog2<decimal::SF_BIT_SIZE>::value;
    decimal t = decimal(1);
    for(int i = 1; i < n && !t.eps(); i++) {
        t = x / (dest * dest);
        dest = dest * 2 + t;
        dest /= 3;
        t -= dest;
    }
    return dest;
}

/**
 * @brief nth root of x
 * @param x
 * @return dest = nthrt(x,n)
 * dest = (dest*(n-1) + x/dest^(n-1))/n
 */

inline decimal nthrt(const decimal& x, decimal::UINT n)
{
    decimal dest = decimal(1);
    dest.SetIndex(x.GetIndex() / n);
    constexpr int count = 10 + ilog2<decimal::SF_BIT_SIZE>::value;
    decimal t = decimal(1);
    for(int i = 1; i < count && !t.eps(); i++) {
        t = x / Power(dest, n - 1);
        dest = dest * (n - 1) + t;
        dest /= n;
        t -= dest;
    }
    return dest;
}

inline decimal factorial(decimal::UINT n)
{
    assert(n >= 0);
    decimal one = decimal(1);
    if(n == 0) {
        return one;
    } else {
        for(decimal::UINT i = 1; i <= n; ++i) {
            one *= i;
        }
    }
    return one;
}

inline decimal __exp(const decimal& x)
{
    advise(std::abs(double(x)) < 1);
    decimal t = x;
    decimal s = t + decimal(1);
    for(int i = 2; !t.eps(1); i++) {
        t *= x;
        t /= i;
        s += t;
    }
    return s;
}

inline decimal exp(int index)
{
    if(index >= 0) {
        return DecimalMath::Power(DecimalMath::E(), index);
    } else {
        const static decimal exp_n1 = DecimalMath::E().ToReciprocal();
        return DecimalMath::Power(exp_n1, -index);
    }
}

inline decimal exp(const decimal& x)
{
    int index = x.ToUINT();
    decimal s = x.GetFraction();
    if(x.isNegative()) {
        index = -index;
        if(double(s) > -0.6) {
            return __exp(s) * DecimalMath::exp(index);
        } else {
            return __exp(decimal(1) + s) * DecimalMath::exp(index - 1);
        }
    } else {
        decimal s = x.GetFraction();
        if(double(s) < 0.6) {
            return __exp(s) * DecimalMath::exp(index);
        } else {
            return __exp(s - decimal(1)) * DecimalMath::exp(index + 1);
        }
    }
}

inline decimal atanh(const decimal& x)
{
    advise(std::abs(double(x)) < 1);
    decimal x2 = x * x;
    decimal xn = x;
    decimal s = x;
    decimal t = decimal(1);
    for(UINT32 i = 3; !t.eps(); i += 2) {
        xn *= x2;
        t = xn / i;
        s += t;
    }
    return s;
}

inline decimal ln(const decimal& x)
{
    decimal y = x.frexpf();
    decimal one = decimal(1);
    if(double(y) < std::sqrt(2.0)) {
        y = (y + one).ToReciprocal();
        y.AddIndex(1);
        y = one - y;
        return atanh(y) * 2 + ln2() * (x.GetIndex());
    } else {
        y.AddIndex(-1);
        y = (y + one).ToReciprocal();
        y.AddIndex(1);
        y = one - y;
        return atanh(y) * 2 + ln2() * (x.GetIndex() + 1);
    }
    // after trans, y <= 3 - 2*sqrt(2)
}

inline decimal log10(const decimal& x)
{
    decimal s = DecimalMath::ln(x);
    s /= DecimalMath::ln10();
    return s;
}

inline decimal log2(const decimal& x)
{
    decimal s = DecimalMath::ln(x.frexpf());
    s /= DecimalMath::ln2();
    s += decimal(x.GetIndex());
    return s;
}

constexpr int triangle_fun_loop_count =
    (sizeof(decimal::UINT) < sizeof(int)) ? std::sqrt(decimal::UINT_T_MAX) - 1 : INT_MAX;

inline decimal sin(const decimal& x);
inline decimal cos(const decimal& x);

inline decimal sin(const decimal& x)
{
    if(double(x) > 3.1419) {
        return -sin(x - PI());
    };
    if(double(x) < -3.1419) {
        return -sin(x + PI());
    };
    if(double(x) > 3.1419 / 2) {
        return sin(PI() - x);
    };
    if(double(x) > 3.1419 / 4) {
        return cos(PI() / 2 - x);
    };
    decimal x2 = x * x;
    decimal t = x;
    decimal s = x;

    for(int i = 2; !t.eps() && (i < triangle_fun_loop_count); i += 2) {
        t *= x2;
        t /= (i * (i + 1));
        t.ChangeSgn();
        s += t;
        //        DEBUG_COUT(i);
    }
    return s;
}

inline decimal sinh(const decimal& x)
{
    advise(std::abs(double(x)) < 1);
    if(x.isZero() == true) {
        return decimal(0);
    }
    decimal x2 = x * x;
    decimal t = x;
    decimal s = x;

    for(int i = 2; !t.eps() && (i < triangle_fun_loop_count); i += 2) {
        t *= x2;
        t /= (i * (i + 1));
        s += t;
    }
    return s;
}

inline decimal cos(const decimal& x)
{
    if(double(x) > 3.1419) {
        return -cos(x - PI());
    };
    if(double(x) < -3.1419) {
        return -cos(x + PI());
    };
    if(double(x) > 3.1419 / 2) {
        return -cos(PI() - x);
    };
    if(double(x) > 3.1419 / 4) {
        return sin(PI() / 2 - x);
    };
    decimal x2 = x * x;
    decimal t = x2 / 2;
    decimal s = decimal(1) - t;

    for(int i = 3; !t.eps() && (i < triangle_fun_loop_count); i += 2) {
        t *= x2;
        t /= (i * (i + 1));
        s += t;
        t.ChangeSgn();
    }
    return s;
}

inline decimal cosh(const decimal& x)
{
    advise(std::abs(double(x)) < 1);
    if(x.isZero() == true) {
        return decimal(1);
    }
    decimal x2 = x * x;
    decimal t = x2 / 2;
    decimal s = decimal(1) + t;

    for(int i = 3; !t.eps() && (i < triangle_fun_loop_count); i += 2) {
        t *= x2;
        t /= (i * (i + 1));
        s += t;
    }
    return s;
}

/**
 * @brief tanh(x) = 1 - 2 / (1 + exp(2*x))
 * @param x
 * @return tanh(x)
 */

inline decimal tanh(const decimal& x)
{
    decimal s = exp(x * 2) + decimal(1);
    decimal c = s.ToReciprocal() * 2;
    return decimal(1) - c;
}

inline decimal tan(const decimal& x)
{
    //    decimal s = sin(x) / cos(x);
    decimal s = sin(x);
    //    DEBUG_COUT(s.ToString(10));
    decimal c = DecimalMath::sqrt(decimal(1) - s * s);
    //    DEBUG_COUT(c.ToString(10));
    s /= c;
    return s;
}

inline decimal _asin(const decimal& x)
{
    assert(x.GetIndex() < 0);
    if(x.isZero() == true) {
        return decimal(0);
    }
    if(double(x) > 0.5) {
        decimal t = decimal(1) - x * x * 2;
        return PI() / 4 - _asin(t) / 2;
    }
    decimal x2 = x * x;
    decimal t = x;
    decimal s = x;
    for(int i = 3; !t.eps(); i += 2) {
        t *= x2;
        t *= (i - 2);
        t /= (i - 1);
        s += (t / i);
    }
    return s;
}

inline decimal asinh(const decimal& x)
{
    decimal t = sqrt(x * x + decimal(1)) + x;
    return ln(t);
}

inline decimal acosh(const decimal& x)
{
    assert(x.GetIndex() >= 0 && x.isPositive());
    decimal t = sqrt(x * x - decimal(1)) + x;
    return ln(t);
}

inline decimal atan(const decimal& x)
{
    double dx = double(x);
    if(std::abs(dx) <= 2 - std::sqrt(3)) {
        assert(std::abs(double(x)) <= 1);
        decimal x2 = x * x;
        decimal xn = x;
        decimal s = x;
        decimal t = decimal(1);
        int x_index = x.GetIndex();

        for(decimal::UINT i = 3; !t.eps(x_index); i += 2) {
            xn *= x2;
            xn.ChangeSgn();
            t = xn / i;
            s += t;
        }
        return s;
    } else {
        if(x.isNegative()) {
            return -atan(decimal::abs(x));
        } else {
            if(std::abs(dx) > 1 + std::sqrt(2.0)) {
                return PI() / 2 - atan(x.ToReciprocal());
            }
            //            constexpr double t = (std::sqrt(2.0) + 1) * (std::sqrt(3.0) - std::sqrt(2.0));
            constexpr double t = 0.767326988;
            if(dx > t) {
                decimal t = (x - decimal(1)) / (x + decimal(1));
                return PI() / 4 + atan(t);
            }
            if(dx > 2 - std::sqrt(3)) {
                decimal t = (x * sqrt3() - decimal(1)) / (x + sqrt3());
                return PI() / 6 + atan(t);
            }
            return decimal(0);
        }
    }
}

/**
 * @brief decimal s = atan(a/b)
 * @param a UINT64
 * @param b UINT64
 * @return
 */

inline decimal atan2(decimal::UINT a, decimal::UINT b)
{
    if(b == 0) {
        return PI() / 2;
    }
    if(a == 0) {
        return decimal(0);
    }
    if(a > b) {
        return PI() / 2 - atan2(b, a);
    }
    constexpr decimal::UINT lvl = decimal::UINT(1) << (decimal::UINT_BIT_SIZE / 2);

    decimal x = decimal(a) / b;
    decimal s = x;
    decimal t = decimal(1);
    if(a < lvl) {
        decimal::UINT a2 = a * a;
        if(b < lvl) {
            decimal::UINT b2 = b * b;
            for(decimal::UINT i = 3; !t.eps(); i += 2) {
                x *= a2;
                x /= b2;
                x.ChangeSgn();
                t = x / i;
                s += t;
            }
        } else {
            for(decimal::UINT i = 3; !t.eps(); i += 2) {
                x *= a2;
                x /= b;
                x /= b;
                x.ChangeSgn();
                t = x / i;
                s += t;
            }
        }
    } else {
        if(b < lvl) {
            decimal::UINT b2 = b * b;
            for(decimal::UINT i = 3; !t.eps(); i += 2) {
                x *= a;
                x *= a;
                x /= b2;
                x.ChangeSgn();
                t = x / i;
                s += t;
            }
        } else {
            for(decimal::UINT i = 3; !t.eps(); i += 2) {
                x *= a;
                x *= a;
                x /= b;
                x /= b;
                x.ChangeSgn();
                t = x / i;
                s += t;
            }
        }
    }
    return s;
}

inline decimal asin(const decimal& x)
{
    assert(std::abs(double(x)) <= 1);
    if(std::abs(double(x)) < std::sqrt(0.5)) {
        decimal t = x / sqrt(decimal(1) - x * x);
        return atan(t);
    } else {
        decimal y = x.ToReciprocal();
        decimal t = sqrt(y * y - decimal(1));
        return PI() / 2 - atan(t);
    }
}

inline decimal acos(const decimal& x)
{
    assert(std::abs(double(x)) <= 1);
    decimal t = PI() / 2 - asin(x);
    //    DEBUG_COUT(double(PI() / 2));
    //    DEBUG_COUT(double(x));
    //    DEBUG_COUT(double(asin(x)));
    //    DEBUG_COUT(double(t));
    return t;
}
} // namespace DecimalMath

#endif // DecimalMath_H
