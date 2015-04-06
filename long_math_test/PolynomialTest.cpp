#include <gtest/gtest.h>
#include <ostream>

#include "Polynomial.h"

using DoublePolynomial = Polynomial<double>;
using namespace std;

class PolynomialTestSuite : public ::testing::Test 
{
};

TEST_F(PolynomialTestSuite, TestZeroConstant) 
{
    DoublePolynomial p = {3};

    EXPECT_EQ(1u, p.size());    
    EXPECT_EQ(3, p(1));    
    EXPECT_EQ(3, p(3));
}

TEST_F(PolynomialTestSuite, TestLinear)
{
    // y(x) = 1 - 3x
    DoublePolynomial p = {1, -3};

    EXPECT_EQ(2u, p.size());    
    EXPECT_EQ(-2, p(1));    
    EXPECT_EQ(-8, p(3));
}

TEST_F(PolynomialTestSuite, TestComplexConstant)
{
    DoublePolynomial p = {1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(1u, p.size());    
    EXPECT_EQ(std::complex<double>(1,0), p(x)); 
}

TEST_F(PolynomialTestSuite, TestComplexLinear)
{
    // y(x) = 1 + 3x
    DoublePolynomial p = {1, 3};
    std::complex<double> x(-1,3);

    EXPECT_EQ(2u, p.size());    
    EXPECT_EQ(std::complex<double>(-2,9), p(x)); 
}

TEST_F(PolynomialTestSuite, TestComplexSquare)
{
    // y(x) = 1 + 3x + x^2
    DoublePolynomial p = {1, 3, 1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(3u, p.size());    
    EXPECT_EQ(std::complex<double>(-10,3), p(x)); 
}

TEST_F(PolynomialTestSuite, Sum)
{
    DoublePolynomial p1 = { 1, 3 ,  1 };
    DoublePolynomial p2 = {-1, 8 , -15};
    DoublePolynomial r  = { 0, 11, -14};

    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << r << std::endl;

    EXPECT_EQ(r, p1+p2); 
}

void test_mult(DoublePolynomial const & p1, DoublePolynomial const & p2, DoublePolynomial const & expected)
{
    DoublePolynomial naive_mult = p1;
    DoublePolynomial fft_mult   = p1;

    naive_mult.naive_multiplication(p2);
    fft_mult.FFT_multiplication(p2);

    EXPECT_EQ(expected, naive_mult);
    EXPECT_EQ(expected, fft_mult);
}

TEST_F(PolynomialTestSuite, Mult1)
{
    DoublePolynomial p1  = { 8 };
    DoublePolynomial p2  = { 1 };
    DoublePolynomial r   = { 8 };
    test_mult (p1 ,p2, r);
}

TEST_F(PolynomialTestSuite, Mult2)
{
    DoublePolynomial p1 = { 1, 2 };
    DoublePolynomial p2 = { 1, 2 };
    DoublePolynomial r  = { 1, 4, 4};
    test_mult (p1 ,p2, r);
}


TEST_F(PolynomialTestSuite, Mult3)
{
    DoublePolynomial p1 = { 1, -2, -19 };
    DoublePolynomial p2 = { 1 };
    DoublePolynomial r  = { 1, -2, -19};
    test_mult (p1 ,p2, r);
}

TEST_F(PolynomialTestSuite, Negate)
{
    DoublePolynomial p = { 1, -2, -19 };
    DoublePolynomial r = {-1,  2,  19 };
    p.negate();    

    EXPECT_EQ(r, p); 
}

TEST_F(PolynomialTestSuite, difference1)
{
    DoublePolynomial p1 = { 1, -2, -19 };
    DoublePolynomial p2 = {-1,  2,  19 };
    DoublePolynomial r  = { 2, -4, -38 };

    EXPECT_EQ(r, p1-p2); 
}

TEST_F(PolynomialTestSuite, IndexOperator)
{
    DoublePolynomial p = { 1, -2, -19 };
    
    EXPECT_EQ( 1 , p[0]);
    EXPECT_EQ(-2 , p[1]);
    EXPECT_EQ(-19, p[2]);

    EXPECT_THROW(p[3], std::out_of_range);
    
    p.resize(4);

    EXPECT_EQ(p[3], 0);
    EXPECT_THROW(p[4], std::out_of_range);
    EXPECT_EQ(4u, p.size());
}

TEST_F(PolynomialTestSuite, BitReverse)
{
    EXPECT_EQ(0u, bit_reverse(0, 8));
    EXPECT_EQ(4u, bit_reverse(1, 8));
    EXPECT_EQ(2u, bit_reverse(2, 8));
    EXPECT_EQ(6u, bit_reverse(3, 8));
    EXPECT_EQ(1u, bit_reverse(4, 8));
    EXPECT_EQ(5u, bit_reverse(5, 8));
    EXPECT_EQ(3u, bit_reverse(6, 8));
    EXPECT_EQ(7u, bit_reverse(7, 8));
}
 
