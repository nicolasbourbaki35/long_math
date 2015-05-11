
#include "LongMath.h"
#include "Polynomial.h"

#include <assert.h>


void remove_trailing_zeros(LongMath::Buffer & v)
{
    auto it = std::find_if  ( v.rbegin()
                             , v.rend()
                             , [](char const & d ) { return d!=0; });
        
    if (it!=v.rbegin()) 
    {
        v.erase(it.base(), v.end());
    }
}

LongMath LongMath::operator+ (const LongMath & lm) const
{
    LongMath new_lm(*this);

    const auto size_l = new_lm.value.size();
    const auto size_r = lm.value.size();
    const auto max_size = std::max(size_l, size_r);
    int carry = 0;

    for (size_t i = 0; i < max_size || carry > 0; ++i)
    {
        const auto left  = (i < size_l) ? new_lm.value[i] : 0;
        const auto right = (i < size_r) ? lm.value[i] : 0; 
        
        const div_t result = div(left + right + carry, 10 );
                
        if (i < size_l)
        {
            new_lm.value[i] = result.rem;
        }
        else
        {
            new_lm.value.push_back(result.rem);
        }

        carry = result.quot;
    }

    return new_lm;    
}

LongMath LongMath::operator- (const LongMath & lm) const
{
    LongMath res;
    
    if (isNegative() != lm.isNegative())
    {
        res = *this + lm;
        res.setSign(sign);
    }
    else
    {
        LongMath const *left = this, *right = &lm;

        if (left->absCompare(*right) == -1)
        { 
            left = &lm;
            right = this;
        }

        res.setSign(left->getSign());

        const auto size_l = left->value.size();
        const auto size_r = right->value.size();
        char carry = 0;

        for (size_t i = 0; i < size_l; ++i)
        {
            char l = left->value[i] - carry;
            char r = (i < size_r) ? right->value[i] : 0; 
            
            if (l < r)
            {
                l += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }
      
            res.value.push_back(l - r);
        }
       
        remove_trailing_zeros(res.value); 
    }
 
    return res;   
}

LongMath LongMath::operator* (const LongMath & right_factor) const
{
    LongMath left_factor(*this);

    if (value.size() > TRIGGER_STRASSEN)
    {
        left_factor.strassenMultiplication(right_factor);
    }
    else if (value.size() > TRIGGER_KARATSUBA)
    {
        left_factor.karatsubaMultiplication(right_factor);
    }
    else
    {
        left_factor.standardMultiplication(right_factor);
    }

    return left_factor;
}

std::ostream & operator<<(std::ostream & os, LongMath const & lm)
{
    if (!lm.value.empty())
    {
        char sign =  (lm.sign == LongMath::Sign::NEG) ? '-' : '+';              
        os << sign;
    }
        auto it = lm.value.rbegin(), it_end = lm.value.rend();
        for(; it != it_end; ++it)
        {
                os << (char)(*it + 48);
        }
        return os;
}

int8_t LongMath::compare(const LongMath & lm) const
{
    bool thisIsZero = this->isZero();
    bool lmIsZero = lm.isZero();

    if (thisIsZero && lmIsZero)
    {
        return 0;
    }
    else if (!thisIsZero && lmIsZero)
    {
        return isNegative() ? -1 : 1;
    }
    else if (thisIsZero && !lmIsZero)
    {
        return lm.isNegative() ? 1 : -1;
    }
    else if (this->isNegative() && !lm.isNegative())
    {
        return -1;
    }
    else if (!this->isNegative() && lm.isNegative())
    {
        return 1;
    }
    else if (this->value.size() < lm.value.size())
    {
        return this->isNegative() ? 1 : -1;
    }
    else if (this->value.size() > lm.value.size())
    {
        return this->isNegative() ? -1 : 1;
    }
    else
    {
        auto it1 = value.rbegin(), it2 = lm.value.rbegin();
        
        for(; it1!=value.rend(); ++it1, ++it2)
        {
            if (*it1 < *it2)
            {
                return this->isNegative() ? 1 : -1;
            }
            else if (*it1 > *it2)
            {
                return this->isNegative() ? -1 : 1;
            }
        }
    }
    
    return 0;
}

int8_t LongMath::absCompare(const LongMath & lm) const
{
    bool thisIsZero = this->isZero();
    bool lmIsZero = lm.isZero();

    if (thisIsZero && lmIsZero)
    {
        return 0;
    }
    else if (!thisIsZero && lmIsZero)
    {
        return 1;
    }
    else if (thisIsZero && lmIsZero)
    {
        return -1;
    }
    else if (this->value.size() < lm.value.size())
    {
        return -1;
    }
    else if (this->value.size() > lm.value.size())
    {
        return 1;
    }
    else
    {
        auto it1 = value.rbegin(), it2 = lm.value.rbegin();
        
        for(; it1!=value.rend(); ++it1, ++it2)
        {
            if (*it1 < *it2)
            {
                return -1;
            }
            else if (*it1 > *it2)
            {
                return 1;
            }
        }
    }
    return 0;
}

bool LongMath::operator< (const LongMath & lm) const
{
    return compare(lm) == -1;
}

bool LongMath::operator> (const LongMath & lm) const
{
    return compare(lm) == 1; 
}

const size_t LongMath::TRIGGER_STRASSEN;
const size_t LongMath::TRIGGER_KARATSUBA;

void multiply(LongMath::Buffer & left_factor, int right_factor);

void LongMath::standardMultiplication(const LongMath & factor) 
{
    LongMath result(0);

    unsigned int index = 0;
    for (; index < factor.value.size(); ++index)
    {
        LongMath tmp = ((*this) * factor.value[index]) << index;
        result = result + tmp;
    }

    if (factor.isNegative())
    {
        result.opposite();
    }
    
    *this = result;
}

void LongMath::strassenMultiplication(const LongMath & right_factor)
{
    Polynomial<double> p1, p2;
    p1.assign(value); p2.assign(right_factor.value);
    
    p1.FFT_multiplication(p2);
   
    // Normalize to 10 basis 
    LongMath result;
    uint8_t carry = 0;
    
    for(size_t i = 0; i<p1.size(); ++i)
    {
        div_t d = div(round(p1[i]) + carry, 10);
        carry = d.quot; 
        result.value.push_back(d.rem);
    }
    
    if(carry != 0)
    {
        result.value.push_back(carry);
    }
    
    *this = result;
    
    remove_trailing_zeros(value);

    if(right_factor.isNegative())
            opposite();
}

void LongMath::karatsubaMultiplication(const LongMath & right_factor)
{  
        LongMath r(right_factor);
        if (r.value.size() < value.size())
        {
            r.value.resize(value.size());
        }
        else if (r.value.size() > value.size())
        {
            value.resize(r.value.size());
        }      
 
        *this = karatsubaRecursive (*this, r);
        
        if(right_factor.isNegative())
            opposite();
    
        remove_trailing_zeros(value);
}

LongMath LongMath::karatsubaRecursive(const LongMath & left_factor, const LongMath & right_factor)
{
    const size_t left_size = left_factor.value.size();
    const size_t right_size = right_factor.value.size();

    if (left_size == 1 && right_size == 1)
    {
        return LongMath(left_factor.value[0] * right_factor.value[0]);
    }
    else if (left_size == 1)
    {
        return right_factor * left_factor.value[0];
    }
    else if (right_size == 1)
    {
        return left_factor * right_factor.value[0];
    }

    size_t split_l = ceil(left_size / 2);
    size_t split_r = ceil(right_size / 2);
    size_t deg = floor(left_size / 2);

    LongMath x2(left_factor.value.begin(), left_factor.value.begin() + split_l);
    LongMath x1(left_factor.value.begin() + split_l, left_factor.value.end());

    LongMath y2(right_factor.value.begin(), right_factor.value.begin() + split_r);
    LongMath y1(right_factor.value.begin() + split_r, right_factor.value.end());

    LongMath a = karatsubaRecursive(x1, y1);
    LongMath c = karatsubaRecursive(x2, y2);
    
    return  c + ((karatsubaRecursive(x1+x2, y1+y2) - a - c) << deg) + (a<< (2 * deg));
}

LongMath LongMath::operator<<(int power) const
{
    auto res = *this;
    res.value.insert(res.value.end(), power, 0);

    auto it = res.value.rbegin();
    auto it_end = res.value.rend() - power;

    for (; it != it_end; ++it)
    {
        std::swap(*it, *(it + power));
    }

    return res;
}

LongMath LongMath::operator* (int right_factor) const
{
    if (right_factor == 0)
    {
        const LongMath result(0);
        return result;
    }
    
    if (right_factor == 1)
    {
        return *this;
    }

    LongMath left_factor(*this);
    multiply(left_factor.value, right_factor);

    return left_factor;
}

void multiply(LongMath::Buffer & left_factor, int right_factor)
{
    int carry = 0;
    LongMath::BufferIt it = left_factor.begin();

    for (; it != left_factor.end(); ++it)
    {
        const int res = *it * right_factor + carry;
        carry = res / 10;
        *it = res % 10;
    }

    if (carry > 0)
    {
        left_factor.push_back(carry);
    }
}

void LongMath::setFromInt(int64_t val)
{
    if (val == 0)
    {
/*        sign = Sign::POS;
        value.push_back(0);*/
        return;
    }
    else if (val < 0)
    {
        val = -val;
        sign = Sign::NEG;
    }
    else
    { 
        sign = Sign::POS;
    }
        
    div_t result = div(val, 10);
    
    while (result.quot != 0 || result.rem != 0)
    {
        value.push_back(result.rem);
        result = div(result.quot, 10);
    }
}

void LongMath::setFromString(std::string const & val)
{
    if (val.empty())
    {
   /*     sign = Sign::POS;
        value.push_back(0);*/
        return;
    }

    auto it = val.rbegin();
    auto it_end = val.rend();
    
    if (val[0] == '-')
    {
        sign = Sign::NEG;
        it_end--;
    }
    else
    {
        sign = Sign::POS;
    }

    for (; it != it_end; ++it)
    {
        if (!isdigit(*it))
            throw std::invalid_argument("Not a digit");

        value.push_back(*it - 48);
    }
}
