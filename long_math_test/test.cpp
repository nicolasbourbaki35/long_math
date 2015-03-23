#include <gtest/gtest.h>
#include <ostream>
#include <fstream>

#include "LongMath.h"

TEST(LongMath, DefaultValue) 
{
    LongMath l;
    EXPECT_EQ(l, 0);
}

TEST(LongMath, InitValue)
{
    LongMath l(5);
    EXPECT_EQ(l, 5);
}

TEST(LongMath, InitTwoDigitValue)
{
    LongMath l(1);
    EXPECT_EQ(l, 1);
}

TEST(LongMath, InitTwoDigitValue2)
{
    LongMath l(12);
    EXPECT_EQ(l, 12);

    LongMath l1(0);
    EXPECT_EQ(l1, 0);
}

TEST(LongMath, InitTwoDigitNegativeValue)
{
    LongMath l(-12);
    EXPECT_EQ(l, -12);
    EXPECT_TRUE(l.isNegative());
}

TEST(LongMath, Dump)
{
    LongMath l(-12);
    std::ostringstream oss;
    
    oss << l;

    EXPECT_EQ(oss.str(), "-12");
}


TEST(LongMath, Addition)
{
    LongMath l(5), k(2);

    EXPECT_EQ(l+k, 7);
}

TEST(LongMath, TwoDigitAddition)
{
    LongMath l(2), k(199);

    EXPECT_EQ(l+k, 201); 
}

TEST(LongMath, InitFromString)
{
    std::string s = "-12563";
    std::string s2 = "1563";

    EXPECT_EQ(LongMath(s), -12563);
    EXPECT_EQ(LongMath(s2), 1563);
}

TEST(LongMath, ReallyLong)
{
    std::ifstream f("input.txt");
    std::string line;
    
    LongMath res;
 
    while (std::getline(f, line))
    {
        LongMath l(line);
        res = res + l;
    }
    
    std::cout << "res=[" << res << "]"  << std::endl;
}    

TEST(LongMath, MultiplyByZero)
{
    LongMath left_factor(5);
    LongMath right_factor(0);
    LongMath result(0);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyByOne)
{
    LongMath left_factor(5);
    LongMath right_factor(1);
    LongMath result(5);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyByTwo)
{
    LongMath left_factor(5);
    LongMath right_factor(2);
    LongMath result(10);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyByMinusOne)
{
    LongMath left_factor(5);
    LongMath right_factor(-1);
    LongMath result(-5);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyByMinusTwo)
{
    LongMath left_factor(5);
    LongMath right_factor(-2);
    LongMath result(-10);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyByTen)
{
    LongMath left_factor(5);
    LongMath right_factor(10);
    LongMath result(50);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, MultiplyBy111)
{
    LongMath left_factor(5);
    LongMath right_factor(111);
    LongMath result(555);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, 4DigitsProduct)
{
    LongMath left_factor(1234);
    LongMath right_factor(9876);
    LongMath result(12186984);

    EXPECT_EQ(left_factor * right_factor, result);
}

TEST(LongMath, ShiftBy1)
{
    LongMath left_factor(12);
    LongMath result(120);

    EXPECT_EQ(left_factor.shift(1), result);
}

TEST(LongMath, ShiftBy4)
{
    LongMath left_factor(12);
    LongMath result(120000);

    EXPECT_EQ(left_factor.shift(4), result);
}

TEST(LongMath, SplitAndSumEven)
{
    LongMath source(1234);
    LongMath result(46);

    EXPECT_EQ(source.splitAndSum(0, 1, 3), result);
}

TEST(LongMath, SplitAndSumOdd)
{
    LongMath source(98765);
    LongMath result(1052);

    EXPECT_EQ(source.splitAndSum(0, 1, 5), result);
}

TEST(LongMath, 12DigitsProduct)
{
    LongMath left_factor("123456789012");
    LongMath right_factor("987654321098");
    LongMath result("121932631136585886175176");

    EXPECT_EQ(left_factor * right_factor, result);
}

