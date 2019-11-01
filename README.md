# GDecimal
GDecimal是采用C++开发的大数计算程序  
开发环境：mingw-w64 + cmake

## 文件说明
* Base.h 实现基础运算，比如带进位的加法_addcarry_u32
* MultBase.h 实现Karatsuba乘法
* FixPointReciprocal.h 实现小数除法，返回A = 1/a，使用A(n+1) = A(n) + A(n)(1 - a*A(n))的迭代格式
* FixPointData.h 存储大数的数据结构，采用定长数组
    ```
    template <typename UINT_T, int MAX_LEN>
    struct FixPointArray {
        ...
        UINT_T Data[MAX_LEN];
        ...
    };
    ```
* Decimal.h 大数类
* DecimalMathConstant.h 大数常数
* DecimalMath.h 大数数学计算
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
    * decimal sinh(const decimal& x)
    * decimal cosh(const decimal& x);
    * ...
* DecimalMathTest.h 数值计算测试
## Source文件夹说明
常见数值的计算结果