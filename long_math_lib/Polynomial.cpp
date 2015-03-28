#include "Polynomial.h"

#include <algorithm>
#include <iostream>
#include <math.h>

void Polynomial::negate()
{
    std::for_each(m_coef.begin(), m_coef.end(), [] (double & c) { c=-c; } );
}

void Polynomial::operator+=(const Polynomial & p)
{
    auto coef1_it  = m_coef.begin();
    auto coef1_end = m_coef.end();
    auto coef2_it  = p.m_coef.begin();
    auto coef2_end = p.m_coef.end();

    for (; (coef1_it != coef1_end) && (coef2_it != coef2_end); ++coef1_it, ++coef2_it)
    {
        *coef1_it += *coef2_it;
    }
}

Polynomial Polynomial::operator+(const Polynomial & p) const
{
    Polynomial tmp(*this);
    tmp+=p; 
    return tmp;
}

void Polynomial::operator-=(const Polynomial & p)
{
    Polynomial tmp(p);
    tmp.negate();
    *this+=tmp;
}

Polynomial Polynomial::operator-(const Polynomial & p) const
{
    Polynomial tmp(p);
    tmp.negate();
    tmp+=*this;

    return tmp;
}
 
void Polynomial::operator*=(const Polynomial & p)
{
    naive_multiplication(p);
}

Polynomial Polynomial::operator*(const Polynomial & p) const
{
    Polynomial tmp(*this);
    tmp*=p;
    return tmp;
}

bool Polynomial::operator==(const Polynomial & p) const
{
    return std::equal(m_coef.cbegin()
                    , m_coef.cend()
                    , p.m_coef.cbegin());
}

void Polynomial::naive_multiplication(const Polynomial & p)
{
    CoefVector tmp_coefs;

    size_t l_size = m_coef.size();
    size_t r_size = p.m_coef.size();

    for(int i=0; i<l_size+r_size-1; ++i)
    {  
        double coef = 0;
        
        for (int j=0; j<i+1; ++j)
        {
            if (i-j<l_size && j<r_size)
            {
                coef += m_coef[i-j] * p.m_coef[j];
            }
        }

        tmp_coefs.push_back(coef);
    }

    m_coef.swap(tmp_coefs);
}    

std::ostream & operator<<(std::ostream & os, Polynomial const & poli)
{
    size_t pow = poli.degree();
    for(auto it = poli.m_coef.rbegin(); it!=poli.m_coef.rend(); ++it)
    {
       if (*it > 0)
          os << '+';
       os << *it << "*x^" << pow-- << " ";
    }
    return os;
}

