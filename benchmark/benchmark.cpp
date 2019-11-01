#include "DecimalMathTest.h"
//-v;-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

int main(int argc, char** argv)
{
    printf("hello world\n");
    decimal a = DecimalMath::__ln2();

    DecimalTest::MachinLike<decimal>(6);
    DecimalTest::chudnovsky<decimal>();

    DecimalTest::TrigonometricFunctionsTest();
    DecimalTest::InverseTrigonometricFunctionsTest();
    DecimalTest::ExpFunctionsTest();
    (DecimalMath::cbrt(a) - decimal(3)).println("cbrt(27) = 3 + ", 10);

    //    DEBUG_COUT(sizeof(int));
    //    DEBUG_COUT(sizeof(int*));
    DEBUG_COUT(ilog2<9>::value);
    DEBUG_COUT(ilog2<101>::value);
    DEBUG_COUT(ilog2<128>::value);
    std::cout << "tan15 * tan 75 = " << (DecimalMath::tan15() * DecimalMath::tan75()).ToString(50) << std::endl;
    return 0;
}