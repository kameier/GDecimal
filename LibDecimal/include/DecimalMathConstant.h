#ifndef DecimalMathConstant_H
#define DecimalMathConstant_H

namespace DecimalMath {
static const decimal& PI()
{
    //        static decimal pi = decimal("3.14159265358979323846264338327950288419716939937510");
    static decimal pi = decimal(
#include "../source/100000_digits_of_pi.txt"
    );
    return pi;
}

static const decimal& E()
{
    static decimal e = decimal(
#include "../source/100000_digits_of_e.txt"
    );
    return e;
}

// ln2 = atanh(decimal(1) / 3) * 2;
static decimal __ln2()
{
    //    decimal x = decimal(1) / 3;
    //    decimal x2 = decimal(1) / 9;
    decimal xn = decimal(1) / 3;
    decimal s = xn;
    decimal t = decimal(1);
    for(UINT32 i = 3; !t.eps(); i += 2) {
        xn /= 9;
        t = xn / i;
        s += t;
    }
    s *= 2;
    return s;
}

static const decimal& ln2()
{
    static decimal _ln2 = decimal(
#include "../source/20000_digits_of_ln2.txt"
    );
    return _ln2;
}

static const decimal& sqrt3()
{
    static decimal s3 = decimal(
#include "../source/20000_digits_of_sqrt3.txt"
    );
    return s3;
}

static const decimal& tan15()
{
    static decimal _tan15 = decimal(2) - sqrt3();
    return _tan15;
}

static const decimal& tan75()
{
    static decimal _tan75 = decimal(2) + sqrt3();
    return _tan75;
}

static const decimal& ln10()
{
    static decimal _ln10 = decimal(
#include "../source/20000_digits_of_ln10.txt"
    );
    return _ln10;
}
}  // namespace DecimalMath

#endif