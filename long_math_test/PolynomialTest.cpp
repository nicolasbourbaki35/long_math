#include <gtest/gtest.h>
#include <ostream>

#include "Polynomial.h"

using DoublePolynomial = Polynomial<double>;
using namespace std;

TEST(DoublePolynomial, TestZeroConstant) 
{
    DoublePolynomial p = {3};

    EXPECT_EQ(p.size(), 1);    
    EXPECT_EQ(p(1), 3);    
    EXPECT_EQ(p(3), 3);
}

TEST(DoublePolynomial, TestLinear)
{
    // y(x) = 1 - 3x
    DoublePolynomial p = {1, -3};

    EXPECT_EQ(p.size(), 2);    
    EXPECT_EQ(p(1), -2);    
    EXPECT_EQ(p(3), -8);
}

TEST(DoublePolynomial, TestComplexConstant)
{
    DoublePolynomial p = {1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.size(), 1);    
    EXPECT_EQ(p(x), std::complex<double>(1,0)); 
}

TEST(DoublePolynomial, TestComplexLinear)
{
    // y(x) = 1 + 3x
    DoublePolynomial p = {1, 3};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.size(), 2);    
    EXPECT_EQ(p(x), std::complex<double>(-2,9)); 
}

TEST(DoublePolynomial, TestComplexSquare)
{
    // y(x) = 1 + 3x + x^2
    DoublePolynomial p = {1, 3, 1};
    std::complex<double> x(-1,3);

    EXPECT_EQ(p.size(), 3);    
    EXPECT_EQ(p(x), std::complex<double>(-10,3)); 
}

TEST(DoublePolynomial, Sum)
{
    DoublePolynomial p1 = { 1, 3 ,  1 };
    DoublePolynomial p2 = {-1, 8 , -15};
    DoublePolynomial r  = { 0, 11, -14};

    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << r << std::endl;

    EXPECT_EQ(p1+p2, r); 
}

TEST(DoublePolynomial, Mult1)
{
    DoublePolynomial p1 = { 1 };
    DoublePolynomial p2 = { 8 };
    DoublePolynomial r  = { 8};

    EXPECT_EQ(p1*p2, r); 
}

TEST(DoublePolynomial, Mult2)
{
    DoublePolynomial p1 = { 1, 2 };
    DoublePolynomial p2 = { 1, 2 };
    DoublePolynomial r  = { 1, 4, 4};
    
    p1*=p2;

    EXPECT_EQ(p1, r); 
}


TEST(DoublePolynomial, Mult3)
{
    DoublePolynomial p1 = { 1, -2, -19 };
    DoublePolynomial p2 = { 1 };
    DoublePolynomial r  = { 1, -2, -19};

    EXPECT_EQ(p1*p2, r); 
}

TEST(DoublePolynomial, Negate)
{
    DoublePolynomial p = { 1, -2, -19 };
    DoublePolynomial r = {-1,  2,  19 };
    p.negate();    

    EXPECT_EQ(p, r); 
}

TEST(DoublePolynomial, difference1)
{
    DoublePolynomial p1 = { 1, -2, -19 };
    DoublePolynomial p2 = {-1,  2,  19 };
    DoublePolynomial r  = { 2, -4, -38 };

    EXPECT_EQ(p1-p2, r); 
}

TEST(DoublePolynomial, IndexOperator)
{
    DoublePolynomial p = { 1, -2, -19 };
    
    EXPECT_EQ(p[0],  1 );
    EXPECT_EQ(p[1], -2 );
    EXPECT_EQ(p[2], -19);

    EXPECT_THROW(p[3], std::out_of_range);
    
    p.resize(4);

    EXPECT_EQ(p[3], 0);
    EXPECT_THROW(p[4], std::out_of_range);
    EXPECT_EQ(p.size(), 4);
}

TEST(DoublePolynomial, BitReverse)
{
    EXPECT_EQ(0, bit_reverse(0, 8));
    EXPECT_EQ(4, bit_reverse(1, 8));
    EXPECT_EQ(2, bit_reverse(2, 8));
    EXPECT_EQ(6, bit_reverse(3, 8));
    EXPECT_EQ(1, bit_reverse(4, 8));
    EXPECT_EQ(5, bit_reverse(5, 8));
    EXPECT_EQ(3, bit_reverse(6, 8));
    EXPECT_EQ(7, bit_reverse(7, 8));
}
 
TEST(DoublePolynomial, FFT)
{
    Polynomial<double> p = {0, 18, -15, 3};
    
    vector<complex<double> > dft = p.FFT<double>();

    vector<complex<double> > res = 
        {   complex<double>( 6 , 0 ), 
            complex<double>( 15, 15), 
            complex<double>(-36, 0 ), 
            complex<double>( 15,-15)    };
    
    EXPECT_EQ(dft.size(), 4);
    
    for(int i=0; i< dft.size();++i)
    {
        /*EXPECT_FLOAT_EQ(dft[i].real(), res[i].real());
        EXPECT_FLOAT_EQ(dft[i].imag(), res[i].imag());*/
        cout << dft[i] << endl;
    }  

}

