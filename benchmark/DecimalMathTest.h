#ifndef DecimalMathTest_H
#define DecimalMathTest_H

#include "../LibDecimal/include/pch.h"

namespace DecimalTest
{
/**
 * @brief calculate pi
 * Chudnovsky algorithm
 * {\frac{1}{\pi}}=12\sum_{k=0}^{\infty}{\frac{(-1)^{k}(6k)!(545140134k+13591409)}{(3k)!(k!)^{3}\left(640320\right)^{3k+3/2}}}
 */

template <typename T> typename std::enable_if_t<!std::is_same<typename T::UINT, UINT64>::value, void> chudnovsky()
{
}

template <typename T> typename std::enable_if_t<std::is_same<typename T::UINT, UINT64>::value, void> chudnovsky()
{
    // log2((6.0 * 2 / 640,320)^3) = -47.1104131
    constexpr UINT64 max_iter = decimal::SF_BIT_SIZE / 47.11 + 1;
    //    static_assert(std::pow(, 3) < std::pow(2, 64), "UINT64 is not for chudnovsky here");
    static_assert(max_iter < 2642246, "UINT64 is not fit for chudnovsky here");

    printf("\nChudnovsky algorithm\n");
    clock_t clock_start = clock();

    decimal a = decimal(1);
    decimal S = decimal(13591409);
    constexpr decimal::UINT b = 10939058860032000ULL;
    //    constexpr UINT64 b = 640320ULL * 640320ULL * (640320ULL / 24UL);
    for(decimal::UINT k = 1; k < max_iter; k++) {
        decimal::UINT p = (6 * k - 1) * (2 * k - 1) * (6 * k - 5);
        decimal::UINT q = k * k * k;
        a /= q;
        a *= p;
        a /= b;
        a.ChangeSgn();
        S += a * (13591409 + 545140134 * k);
    }
    decimal pi = DecimalMath::sqrt(decimal(10005)) / S * 426880;
    // 3*5*67 = 1005
    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    (pi - DecimalMath::PI()).println("err in calc PI:", 10);
    printf("Time Elapsed: %f second\n\n", total_time);
    return;
}

/**
 * @brief calculate pi
 * Machin-like formula
 * {\frac{\pi}{4}}=\sum _{{n=0}}^{N-1}c_{n}\arctan {\frac  {a_{n}}{b_{n}}}
 */
template <typename T> typename std::enable_if_t<sizeof(typename T::UINT) < sizeof(UINT32), void> MachinLike(int num)
{
}
template <typename T> typename std::enable_if_t<sizeof(typename T::UINT) >= sizeof(UINT32), void> MachinLike(int num)
{
    printf("\nSTART calc PI with Machin-like formula\n");
    std::vector<int> cn;
    std::vector<decimal::UINT> an, bn;
    switch(num) {
        case(1): {
            cn = { 1 };
            an = { 1 };
            bn = { 1 };
            break;
        }
        case(2): {
            cn = { 4, -1 };
            an = { 1, 1 };
            bn = { 5, 239 };
            break;
        }
        case(3): {
            cn = { 44, 139, -12 };
            an = { 74684, 1, 20138 };
            bn = { 14967113, 239, 15351991 };
            break;
        }
        case(4): {
            cn = { 44, 7, -12, 24 };
            an = { 1, 1, 1, 1 };
            bn = { 57, 239, 682, 12943 };
            break;
        }
        case(6): {
            cn = { 183, 32, -68, 12, -12, -100 };
            an = { 1, 1, 1, 1, 1, 1 };
            bn = { 239, 1023, 5832, 110443, 4841182, 6826318 };
            break;
        }
        default: {
        }
    }

    clock_t clock_start = clock();
    decimal pi = decimal(0);
    //    DEBUG_COUT(clock_start);
    double ppi = 0;
    for(int i = 0; i < (int)an.size(); i++) {
        decimal t = DecimalMath::atan2(an[i], bn[i]);
        //                decimal t = DecimalMath::atan(decimal(an[i])/bn[i]);
        //        DEBUG_COUT(bn[i]);
        //        DEBUG_COUT(t.ToString(10));
        //        DEBUG_COUT(t.ToDouble() - atanl(double(an[i]) / bn[i]));
        t *= cn[i];
        //                DEBUG_COUT(t.ToString(10));
        pi += t;
        ppi += cn[i] * atanl(double(an[i]) / bn[i]);
    }
    pi *= 4;
    ppi *= 4;
    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    pi.println("pi:", 100);
    //    PI().println("PI:", 100);
    //    DEBUG_COUT(ppi);
    //    DEBUG_COUT(pi.ToDouble());
    (pi - DecimalMath::PI()).println("err in calc PI:", 10);
    printf("\nTime Elapsed: %f second\n", total_time);
    printf("END calc PI\n");
    pi.TEST_ToString("pi", 10);
}

void TrigonometricFunctionsTest()
{
    printf("\nTrigonometric Functions Test\n");
    clock_t clock_start = clock();

    decimal sqrt3 = DecimalMath::sqrt(decimal(3));
    sqrt3.println("sqrt(3) = ", 10);
    (sqrt3 * sqrt3 - decimal(3)).println("sqrt(3)^2 = 3 + ", 10);

    decimal a = DecimalMath::sin(DecimalMath::PI() / 3) - sqrt3 / 2;
    //    DEBUG_COUT(a.ToDouble());
    a.println("sin(pi/3) = sqrt(3)/2 + ", 10);

    decimal b = DecimalMath::sin(DecimalMath::PI() / 2) - decimal(1);
    b.println("sin(pi/2) = 1 + ", 10);

    decimal c = DecimalMath::sin(DecimalMath::PI() / 6) - decimal(1) / 2;
    c.println("sin(pi/6) = 1/2 + ", 10);

    decimal d = DecimalMath::cos(DecimalMath::PI() / 2);
    d.println("cos(pi/2) = 0 + ", 10);

    decimal e = DecimalMath::cos(DecimalMath::PI() / 3) - decimal(1) / 2;
    e.println("cos(pi/3) = 1/2 + ", 10);

    decimal f = DecimalMath::cos(DecimalMath::PI() / 6) - sqrt3 / 2;
    f.println("cos(pi/6) = sqrt(3)/2 + ", 10);

    decimal g = DecimalMath::tan(DecimalMath::PI() / 4) - decimal(1);
    g.println("tan(pi/4) = 1 + ", 10);

    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    printf("\nTime Elapsed: %f second\n", total_time);
}

void InverseTrigonometricFunctionsTest()
{
    printf("\nInverse Trigonometric Functions Test\n");
    clock_t clock_start = clock();

    decimal h0 = decimal("0.6347");
    decimal h1 = DecimalMath::sin(DecimalMath::asin(h0)) - h0;
    h1.println("sin(asin(h0)) = h0 + ", 10);

    decimal h2 = DecimalMath::asin(DecimalMath::sin(h0)) - h0;
    h2.println("asin(sin(h0)) = h0 + ", 10);

    decimal h3 = DecimalMath::acos(DecimalMath::cos(h0)) - h0;
    h3.println("acos(cos(h0)) = h0 + ", 10);

    decimal h4 = DecimalMath::cos(DecimalMath::acos(h0)) - h0;
    h4.println("cos(acos(h0)) = h0 + ", 10);

    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    printf("\nTime Elapsed: %f second\n", total_time);
}

void ExpFunctionsTest()
{
    printf("\nExp Functions Test\n");
    clock_t clock_start = clock();

    decimal n = decimal(11) / 2;
    //    DEBUG_COUT(n.ToDouble());
    decimal h = DecimalMath::exp(decimal(0) - n) * DecimalMath::exp(n) - decimal(1);
    h.println("exp(n) * exp(-n) = 1 + ", 10);

    int m = 50;
    decimal i = DecimalMath::exp(-m) * DecimalMath::exp(decimal(m)) - decimal(1);
    i.println("exp(m) * exp(-m) = 1 + ", 10);

    int p = -29;
    decimal j = DecimalMath::ln(DecimalMath::Power(DecimalMath::exp(1), p)) - decimal(p);
    j.println("ln(exp(p)) = p + ", 10);

    decimal q = decimal(61) / 7;
    decimal k = DecimalMath::exp(DecimalMath::ln(q)) - q;
    k.println("exp(ln(q)) = q + ", 10);

    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    printf("\nTime Elapsed: %f second\n", total_time);
}

} // namespace DecimalTest

#endif