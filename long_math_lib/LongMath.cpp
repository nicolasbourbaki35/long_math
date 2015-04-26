
#include "LongMath.h"
#include "Polynomial.h"

// ******************************  public  ********************************

LongMath LongMath::operator+ (const LongMath & lm) const
{
    if (lm.isNegative() != isNegative())
        return this->operator-(lm);    

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
    // TODO:
    return lm;   
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

// ******************************  private  ********************************

const size_t LongMath::TRIGGER_STRASSEN;
const size_t LongMath::TRIGGER_KARATSUBA;

void multiply(LongMath::Buffer & left_factor, int right_factor);

void LongMath::standardMultiplication(const LongMath & factor) 
{
    LongMath result(0);

    unsigned int index = 0;
    for (; index < factor.value.size(); ++index)
    {
        LongMath tmp = (*this) * factor.value[index];
        tmp.shift(index);

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

    if(right_factor.isNegative())
            opposite();
}

void LongMath::karatsubaMultiplication(const LongMath & right_factor)
{   
    if (right_factor.isZero())
    {
        *this = right_factor;
    }
    else if (isZero())
    {
        return;
    }
    else
    {
        *this = karatsubaRecursive (*this, right_factor);
        
        if(right_factor.isNegative())
            opposite();
    }
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

    LongMath x1(left_factor.value.begin(), left_factor.value.begin() + split_l);
    LongMath x2(left_factor.value.begin() + split_l, left_factor.value.end());

    LongMath y1(right_factor.value.begin(), right_factor.value.begin() + split_r);
    LongMath y2(right_factor.value.begin() + split_r, right_factor.value.end());

    LongMath sum1 = x1 + x2;
    LongMath sum2 = y1 + y2;

    LongMath a = karatsubaRecursive(x1, y1) * pow10(2 * deg);
    LongMath c = karatsubaRecursive(x2, y2);

    return a + c  
           + (karatsubaRecursive(sum1, sum2) /*- a - c*/) * pow10(deg);
}

LongMath & LongMath::shift(int power)
{
    value.insert(value.end(), power, 0);

    auto it = value.rbegin();
    auto it_end = value.rend() - power;

    for (; it != it_end; ++it)
    {
        std::swap(*it, *(it + power));
    }

    return *this;
}

LongMath LongMath::splitAndSum(size_t min_index, size_t index, size_t max_index) const
{
    LongMath splitted;

    const auto min_high_index = index + (max_index - min_index) % 2;
    const auto size_h = max_index - min_high_index + 1;
    const auto size_l = index - min_index + 1;
    const auto splitted_size = std::max(size_l, size_h);
    int carry = 0;

    for (size_t i = 0; i < splitted_size || carry > 0; ++i)
    {
        const auto high = (i < size_h) ? value[min_high_index + i] : 0;
        const auto low  = (i < size_l) ? value[min_index + i] : 0; 
        
        const div_t result = div(high + low + carry, 10);
                
        splitted.value.push_back(result.rem);
        carry = result.quot;
    }

    return splitted;
}


LongMath LongMath::operator* (int right_factor) const
{
    if (right_factor == 0)
    {
        const LongMath result(0);
        return result;
    }
    
    LongMath left_factor(*this);

    if (right_factor == 1)
    {
        return left_factor;
    }

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

void LongMath::setFromInt(int val)
{
    if (val < 0)
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
        sign = Sign::POS;
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
        value.push_back(*it - 48);
    }
}
