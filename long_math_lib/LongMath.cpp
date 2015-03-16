#include "LongMath.h"

// ******************************  public  ********************************

LongMath LongMath::operator+ (const LongMath & lm) const
{
    LongMath new_lm(*this);
    
    const auto size_l = new_lm.value.size();
    const auto size_r = lm.value.size();
    const auto max_size = std::max(size_l, size_r);
    int carry = 0;

    // TODO : If either this or lm is negative
    
    for (auto i = 0; i < max_size || carry > 0; ++i)
    {
        const auto left  = (i < size_l) ? new_lm.value[i] : 0;
        const auto right = (i < size_r) ? lm.value[i] : 0; 
        
        const div_t result  = div(left + right + carry, 10 );
                
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


LongMath LongMath::operator* (const LongMath & right_factor) const
{
    const LongMath left_factor(*this);
    LongMath result;

    if (value.size() > TRIGGER_STRASSEN)
    {
        result = strassenMultiplication(left_factor, right_factor);
    }
    else if (value.size() > TRIGGER_KARATSUBA)
    {
        result = karatsubaMultiplication(left_factor, right_factor);
    }
    else
    {
        result = standardMultiplication(left_factor, right_factor);
    }

    return result;
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



LongMath LongMath::standardMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    LongMath result(0);

    unsigned int index = 0;

    for (; index < left_factor.value.size(); ++index)
    {
        LongMath tmp = right_factor * left_factor.value[index];
        tmp.shift(index);

        result = result + tmp;
    }

    return result;
}

LongMath LongMath::strassenMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    return left_factor;
}

LongMath LongMath::karatsubaMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    return left_factor;

    /*
    if (left_factor.value.size() < 2 || right_factor.value.size() < 2)
        return standardMultiplication(left_factor, right_factor);

    const size_t middle_point = min(left_factor.value.size(), right_factor.value.size()) / 2;
    */
}

LongMath & LongMath::shift(int power)
{
    // TODO
    return *this;
}

LongMath LongMath::splitAt(size_t index) const
{
    LongMath splitted(*this);

    // TODO
    return splitted;
}


LongMath LongMath::operator* (int right_factor) const
{
    const LongMath left_factor(*this);
    
    // TODO

    return left_factor;   
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
    int i = 0;
    
    while (result.quot != 0 || result.rem != 0)
    {
        value.push_back(result.rem);
        result = div(result.quot, 10);
    }
}

void LongMath::setFromString(std::string const & val)
{
    if (val.empty())
        return;

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