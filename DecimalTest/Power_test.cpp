#include "../LibDecimal/include/pch.h"
//-v;-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

int test4()
{
    for(int i = 0; i < 20; ++i)
    {
        //        std::cout << "LINE" << __LINE__ << ":" << i << std::endl;
        printf_s("\n  Line %d : %d \n", __LINE__, i);
        decimal ten = decimal(10);
        decimal tmp1 = DecimalMath::Power(ten, i);
        decimal tmp2 = DecimalMath::Power(ten, -i);

        printf("10^(-%d) = %e\n", i, double(tmp1));
        //        tmp1.println("tmp1:");
        printf("10^(%d) = %e\n", i, double(tmp2));
        //        tmp2.println("tmp2:");
        printf("err: %e\n", double(decimal(1) - tmp1 * tmp2));
    }
    return 1;
}

int test5()
{
    decimal x = decimal(rand());
    for(int i = 0; i < 1e6; ++i)
    {
        decimal y = decimal(rand());
        decimal z = x * y;
    }
    return 1;
}

int Power_test()
{
    test4();
    return 0;
}
