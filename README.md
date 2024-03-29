# GDecimal
GDecimal是采用C++开发的大数计算程序  
开发环境：mingw-w64 + cmake + vscode

## include文件夹说明
* Base.h                实现基础运算，比如带进位的加法_addcarry_u32
* MultBase.h            实现Karatsuba乘法
* FixPointReciprocal.h  实现小数除法，返回A = 1/a，使用A(n+1) = A(n) + A(n)(1 - a*A(n))的迭代格式
* FixPointData.h        存储大数的数据结构，采用定长数组
    ```
    template <typename UINT_T, int MAX_LEN>
    struct FixPointArray {
        ...
        UINT_T Data[MAX_LEN];
        ...
    };
    ```
* Decimal.h             大数类
* DecimalMathConstant.h 大数常数
    * 部分数值来自Source文件夹内的txt文件
* DecimalMath.h         大数数学计算
    * template <typename T> T Power(T base, int exponent);
    * decimal sqrt(const decimal& x);
    * decimal cbrt(const decimal& x);
    * decimal nthrt(const decimal& x, decimal::UINT n);
    * decimal factorial(decimal::UINT n);
    * decimal exp(int index);
    * decimal atanh(const decimal& x);
    * decimal ln(const decimal& x);
    * decimal sin(const decimal& x);
    * decimal cos(const decimal& x);
    * decimal sinh(const decimal& x);
    * decimal cosh(const decimal& x);
    * ...

## test文件夹说明
* test.cpp              简单的测试用例
    * 第三方库 GoogleTest

## benchmark文件夹说明
* DecimalMathTest.h     数值计算测试,实现圆周率pi的计算
    * Chudnovsky algorithm

        $${\frac{1}{\pi}}=12\sum_{k=0}^{\infty}{\frac{(-1)^{k}(6k)!(545140134k+13591409)}{(3k)!(k!)^{3}\left(640320\right)^{3k+3/2}}}$$

    * Machin-like formula

        $${\frac{\pi}{4}}=\sum _{{n=0}}^{N-1}c_{n}\arctan {\frac  {a_{n}}{b_{n}}}$$
        
* benchmark.cpp         性能测试

## Source文件夹说明
常见数值的计算结果
* ln2：  20000_digits_of_ln2.txt
* ln10： 20000_digits_of_ln10.txt
* sqrt3: 20000_digits_of_sqrt3.txt
* e:     100000_digits_of_e.txt
* pi:    100000_digits_of_pi.txt