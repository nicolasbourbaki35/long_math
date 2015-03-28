#include <gtest/gtest.h>
#include <ostream>

#include "Polynomial.h"

TEST(Polynomial, TestZeroConstant) 
{
    Polynomial p = {3};

    EXPECT_EQ(p.degree(), 0);    
    EXPECT_EQ(p(1), 3);    
    EXPECT_EQ(p(3), 3);
}

TEST(Polynomial, TestLinear)
{
    // y(x) = 1 - 3x
    Polynomial p = {1, -3};

    EXPECT_EQ(p.degree(), 1);    
    EXPECT_EQ(p(1), -2);    
    EXPECT_EQ(p(3), -8);
}

TEST(Polynomial, TestComplexConstant)
{
    Polynomial p = {1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.degree(), 0);    
    EXPECT_EQ(p(x), std::complex<double>(1,0)); 
}

TEST(Polynomial, TestComplexLinear)
{
    // y(x) = 1 + 3x
    Polynomial p = {1, 3};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.degree(), 1);    
    EXPECT_EQ(p(x), std::complex<double>(-2,9)); 
}

TEST(Polynomial, TestComplexSquare)
{
    // y(x) = 1 + 3x + x^2
    Polynomial p = {1, 3, 1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.degree(), 2);    
    EXPECT_EQ(p(x), std::complex<double>(-10,3)); 
}

TEST(Polynomial, Sum)
{
    Polynomial p1 = { 1, 3 ,  1 };
    Polynomial p2 = {-1, 8 , -15};
    Polynomial r  = { 0, 11, -14};

    EXPECT_EQ(p1+p2, r); 
}

TEST(Polynomial, Mult1)
{
    Polynomial p1 = { 1 };
    Polynomial p2 = { 8 };
    Polynomial r  = { 8};

    EXPECT_EQ(p1*p2, r); 
}

TEST(Polynomial, Mult2)
{
    Polynomial p1 = { 1, 2 };
    Polynomial p2 = { 1, 2 };
    Polynomial r  = { 1, 4, 4};
    
    p1*=p2;

    EXPECT_EQ(p1, r); 
}


TEST(Polynomial, Mult3)
{
    Polynomial p1 = { 1, -2, -19 };
    Polynomial p2 = { 1 };
    Polynomial r  = { 1, -2, -19};

    EXPECT_EQ(p1*p2, r); 
}

TEST(Polynomial, Negate)
{
    Polynomial p = { 1, -2, -19 };
    Polynomial r = {-1,  2,  19 };
    p.negate();    

    EXPECT_EQ(p, r); 
}

TEST(Polynomial, difference1)
{
    Polynomial p1 = { 1, -2, -19 };
    Polynomial p2 = {-1,  2,  19 };
    Polynomial r  = { 2, -4, -38 };

    EXPECT_EQ(p1-p2, r); 
}

