#include <gtest/gtest.h>
#include <ostream>
#include <fstream>
#include <limits>

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
    
//    std::cout << "res=[" << res << "]"  << std::endl;
}    

void test_mult(LongMath const & l1, LongMath const &l2, LongMath const & res, const char * scope)
{
    LongMath r1(l1), r2(l1), r3(l1);

    r1.standardMultiplication(l2);
    r2.karatsubaMultiplication(l2);
    r3.strassenMultiplication(l2);
   
    SCOPED_TRACE(scope);

    { 
        SCOPED_TRACE("standard mult failed");
        EXPECT_EQ(res, r1);
    }
    
    {
        SCOPED_TRACE("karatsuba mult failed");
        EXPECT_EQ(res, r2);
    }
    
    {
        SCOPED_TRACE("strassen mult failed");
        EXPECT_EQ(res, r3);
    }
}

TEST(LongMath, MultiplyByZero)
{
    LongMath left_factor(5);
    LongMath right_factor(0);
    LongMath result(0);

    test_mult(left_factor, right_factor, result, "MultiplyByZero");
}

TEST(LongMath, MultiplyByOne)
{
    LongMath left_factor(5);
    LongMath right_factor(1);
    LongMath result(5);

    test_mult(left_factor, right_factor, result, "MultiplyByOne");
}

TEST(LongMath, MultiplyByTwo)
{
    LongMath left_factor(5);
    LongMath right_factor(2);
    LongMath result(10);

    test_mult(left_factor, right_factor, result, "MultiplyByTwo");
}

TEST(LongMath, MultiplyByMinusOne)
{
    LongMath left_factor(5);
    LongMath right_factor(-1);
    LongMath result(-5);

    test_mult(left_factor, right_factor, result, "MultiplyByMinusOne");
}

TEST(LongMath, MultiplyByMinusTwo)
{
    LongMath left_factor(5);
    LongMath right_factor(-2);
    LongMath result(-10);

    test_mult(left_factor, right_factor, result, "MultiplyByMinusTwo");
}

TEST(LongMath, MultiplyByTen)
{
    LongMath left_factor(5);
    LongMath right_factor(10);
    LongMath result(50);

    test_mult(left_factor, right_factor, result, "MultiplyByTen");
}

TEST(LongMath, MultiplyBy111)
{
    LongMath left_factor(5);
    LongMath right_factor(111);
    LongMath result(555);

    test_mult(left_factor, right_factor, result, "MultiplyByTen");
}

TEST(LongMath, 2DigitsProduct)
{
    LongMath left_factor(47);
    LongMath right_factor(78);
    LongMath result(3666);

    test_mult(left_factor, right_factor, result, "2DigitsProduct");
}


TEST(LongMath, 4DigitsProduct)
{
    LongMath left_factor(1234);
    LongMath right_factor(5678);
    LongMath result(7006652);

    test_mult(left_factor, right_factor, result, "4DigitsProduct");
}

TEST(LongMath, ShiftBy1)
{
    LongMath left_factor(12);
    LongMath result(120);

    EXPECT_EQ(left_factor<<1, result);
}

TEST(LongMath, ShiftBy4)
{
    LongMath left_factor(12);
    LongMath result(120000);

    EXPECT_EQ(left_factor<<4, result);
}

TEST(LongMath, Compare)
{
    LongMath l1(9), l2(2);
    EXPECT_TRUE(l2 < l1);
    EXPECT_EQ(l2.compare(l1), -1);

    LongMath l3(-9), l4(2);
    EXPECT_TRUE(l3 < l4);
    EXPECT_EQ(l3.compare(l4), -1);
    EXPECT_EQ(l3.absCompare(l4), 1);

    LongMath l5(9), l6(-2);
    EXPECT_TRUE(l6 < l5);
    EXPECT_EQ(l5.compare(l6), 1);
    EXPECT_EQ(l5.absCompare(l6), 1);

    LongMath l7(-9), l8(-2);
    EXPECT_TRUE(l7 < l8);
    EXPECT_EQ(l7.compare(l8), -1);
    EXPECT_EQ(l7.absCompare(l8), 1);

    LongMath l9(-900000), l10(-900000);
    EXPECT_FALSE(l9  < l10);
    EXPECT_FALSE(l9  > l10);
    EXPECT_TRUE(l9 == l10);
    EXPECT_EQ(l9.compare(l10), 0);
    EXPECT_EQ(l9.absCompare(l10), 0);
}

TEST(LongMath, Subtract)
{
    LongMath l1(-9), l2(-2), r1(-7);
    EXPECT_EQ(r1, l1-l2);

    LongMath l3(9), l4(-2), r2(11);
    EXPECT_EQ(r2, l3-l4);

    LongMath l5(-9), l6(2), r3(-11);
    EXPECT_EQ(r3, l5-l6);
 
    LongMath l7(9), l8(2), r4(7);
    EXPECT_EQ(r4, l7-l8);
   
    LongMath l9(90000), l10(1000), r5(89000);
    EXPECT_EQ(r5, l9-l10);
    
    LongMath l11(90000), l12(1), r6(89999);
    EXPECT_EQ(r6, l11-l12);

    LongMath l13(1000), l14(1), r7(999);
    EXPECT_EQ(r7, l13-l14);
  
    LongMath l15(1000), l16(999), r8(1);
    EXPECT_EQ(r8, l15-l16);
}

TEST(LongMath, IsZero)
{
   EXPECT_TRUE(LongMath(0).isZero());
   EXPECT_FALSE(LongMath(1).isZero());
   EXPECT_FALSE(LongMath(-1).isZero());
   EXPECT_TRUE(LongMath("0").isZero());
   EXPECT_TRUE(LongMath("0000").isZero());
}

TEST(LongMath, 12DigitsProduct)
{
    /*LongMath left_factor("123456789012");
    LongMath right_factor("987654321098");
    LongMath result("121932631136585886175176");
    
    test_mult(left_factor, right_factor, result, "12DigitsProduct");
*/
}

TEST(LongMath, ManyProducts)
{
    //#pragma omp parallel for
    /*for(int i = 0; i<2000; ++i)
    {
        for(int j = i; j<2000; ++j)
        {
            LongMath l1(i);
            l1.karatsubaMultiplication(LongMath(j));

            EXPECT_EQ(LongMath(i*j), l1);
        }
    }*/
}


