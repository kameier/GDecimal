#include "../LibDecimal/include/pch.h"
//-v;-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
int test1()
{
    decimal a = decimal("33220.027832016345435425216");
    std::cout << a << std::endl;
    //    a.println("a:", 50);
    //    DEBUG_COUT(double(a));
    a.floor().println("a floor:", 20);
    a.round().println("a round:", 20);

    decimal b = decimal("-99999947789089089089.6349863498634986349863498");
    b.println("b:", 50);
    //    DEBUG_COUT(b.ToDouble());
    b.floor().println("b floor:", 60);
    b.round().println("b round:", 60);

    decimal c = a / b;
    c.println("c:", 50);

    decimal d = a + b - b;
    d.println("d:", 50);

    decimal f = decimal("0123456789012345678901234567890.1234567891123456789012345678901234567890");
    //    //    f = decimal("123455.12345678901234567890");
    f.println("f:", 60);
    f.TEST_ToString("f", 10);
    return 0;
}

int test2()
{
    std::cout << std::endl;
    decimal n = decimal(0);
    decimal one = decimal(1);
    for(int i = 1; i < 20; ++i)
    {
        n = n + one;
        printf("Integer %d:\n", i);
        n.ToReciprocal().println("Reciprocal:", 30);
        decimal tmp = one - n * n.ToReciprocal();
        tmp.println("tmp:", 10);
    }
    return 0;
}

int test3()
{
    char Estr[] = "2.718281828459045235360287471352";
    decimal E = decimal(Estr);
    E.println("E :");
    return 1;
}

int IO_test()
{
    test1();
    test2();
    return 0;
}