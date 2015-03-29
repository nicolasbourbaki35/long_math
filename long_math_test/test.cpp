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
    
//    std::cout << "res=[" << res << "]"  << std::endl;
}    

