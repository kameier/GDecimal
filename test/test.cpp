#include <iostream>

#include "../include/pch.h"

#include <gtest/gtest.h>

class TestCase : public testing::Test
{
public:
    // Add log
    static void SetUpTestCase()
    {
        std::cout << "SetUpTestCase" << std::endl;
    }
    static void TearDownTestCase()
    {
        std::cout << "TearDownTestCase" << std::endl;
    }
    // SetUp Run before TEST
    virtual void SetUp()
    {
        std::cout << "SetUp" << std::endl;
    }
    // TearDown Run after TEST
    virtual void TearDown()
    {
        std::cout << "TearDown" << std::endl;
    }
};

TEST_F(TestCase, Power)
{
    char str[1000];
    int index = 3;
    for(int i = 0; i < 20; ++i) {
        //        std::cout << "LINE" << __LINE__ << ":" << i << std::endl;
        //        printf_s("\n  Line %d : %d \n", __LINE__, i);
        decimal ten = decimal(10);
        decimal tmp1 = DecimalMath::Power(ten, i);
        decimal tmp2 = DecimalMath::Power(ten, -i);
        decimal err = decimal::abs(decimal(1) - tmp1 * tmp2);

        sprintf(str, "%d : 1 - %e * %e = %e with index = %d\n", i, double(tmp1), double(tmp2), double(err),
            err.GetIndex() + decimal::SF_BIT_SIZE);

        ASSERT_TRUE(err.eps(index)) << str;
    }
}

TEST_F(TestCase, StringIO)
{
    std::string a_in = "33220.027832016345435425216";
    decimal a = decimal(a_in.c_str());
    std::string a_out = a.ToString();
    ASSERT_TRUE(strncmp(a_out.c_str(), a_in.c_str(), a_in.length() - 1) == 0);

    decimal b = decimal("-99999947789089089089.6349863498634986349863498");
    std::string b_floor = "-99999947789089089089";
    ASSERT_TRUE(strncmp(b.floor().ToString().c_str(), b_floor.c_str(), b_floor.length()) == 0);

    std::string b_round = "-99999947789089089090";
    ASSERT_TRUE(strncmp(b.round().ToString().c_str(), b_round.c_str(), b_round.length()) == 0);

    decimal c = a / b;
    std::string c_out = "-3.322004517650453710438500448401543616713250864397E-16";
    ASSERT_TRUE(strncmp(c.ToString(50).c_str(), c_out.c_str(), c_out.length()) == 0);
    //    c.println("c:", 50);

    decimal d = a + b - b;
    std::string d_out = "33220.027832016345435425215999999999999999999999999";
    ASSERT_TRUE(strncmp(d.ToString(50).c_str(), d_out.c_str(), d_out.length()) == 0);

    decimal f = decimal("0123456789012345678901234567890.1234567891123456789012345678901234567890");
    //    f.println("f:", 60);
    decimal f_err = f.TEST_ToString("f", 10);
    ASSERT_TRUE(f_err.eps(10));
}

TEST_F(TestCase, MathConstant)
{
    decimal l2 = DecimalMath::__ln2();
    std::string l2_text = {
#include "../source/20000_digits_of_ln2.txt"
    };
    std::string l2_out = l2.ToString(60);
    ASSERT_TRUE(strncmp(l2_text.c_str(), l2_out.c_str(), l2_out.length()) == 0);

    decimal t15 = DecimalMath::tan15();
    std::string t15_out = "0.26794919243112";
    ASSERT_TRUE(strncmp(t15.ToString().c_str(), t15_out.c_str(), t15_out.length()) == 0);

    decimal t75 = DecimalMath::tan75();
    //    t75.println("tan75:");
    std::string t75_out = "3.732050807568877293527446341505872366942805253810380628055806979451933016";
    ASSERT_TRUE(strncmp(t75.ToString().c_str(), t75_out.c_str(), t75_out.length()) == 0);

    decimal E = DecimalMath::E();
    std::string E_out = E.ToString(100);
    std::string E_text = {
#include "../source/100000_digits_of_e.txt"
    };
    ASSERT_TRUE(strncmp(E_text.c_str(), E_out.c_str(), E_out.length()) == 0);

    decimal E_err = E.TEST_ToString("e", 10);
    //    std::cout << "E_err index " << (E_err.GetIndex() + decimal::SF_BIT_SIZE) << std::endl;
    ASSERT_TRUE(E_err.eps(15));
}

TEST_F(TestCase, Reciprocal)
{
    decimal n = decimal(0);
    decimal one = decimal(1);
    char str[1000];
    for(int i = 1; i < 20; ++i) {
        n = n + one;
        decimal err = one - n * n.ToReciprocal();
        sprintf(str, "%d : 1 - (%d) * (%d^-1) = %e with index = %d\n", i, i, i, double(err),
            err.GetIndex() + decimal::SF_BIT_SIZE);

        ASSERT_TRUE(err.eps(2)) << str;
    }
}

TEST_F(TestCase, e_calc)
{
    decimal E = DecimalMath::E();
    // 获取的是计算机启动后的时间间隔,得到的是CPU时间,精确到1/CLOCKS_PER_SEC秒。
    clock_t clock_start = clock();
    decimal e = decimal(1);
    decimal t = decimal(1);
    //        DEBUG_COUT((f/1).ToString(10));
    //        DEBUG_COUT((f/2).ToString(10));
    //        DEBUG_COUT((f/3).ToString(10));
    int i;
    for(i = 1; !t.eps(); ++i) {
        // t *= decimal(i).ToReciprocal();
        t /= i;
        e += t;
    }
    int count = i;

    decimal f = DecimalMath::factorial(count).ToReciprocal();

    clock_t clock_end = clock();
    double total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
    printf("\nTime Elapsed: %f second\n", total_time);

    decimal err = e - E;
    ASSERT_LT(double(err), 1e-3);

    ASSERT_TRUE((err - f).isNegative());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv); // Init gtest
    return RUN_ALL_TESTS();               // RUN_ALL_TESTS()
}