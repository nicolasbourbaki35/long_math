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

void multiply(LongMath::Buffer & left_factor, int right_factor);

LongMath LongMath::standardMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    LongMath result(0);

    unsigned int index = 0;
    for (; index < right_factor.value.size(); ++index)
    {
        LongMath tmp = left_factor * right_factor.value[index];
        tmp.shift(index);

        result = result + tmp;
    }

    if (right_factor.isNegative())
    {
        result.opposite();
    }

    return result;
}


/*
LonhMath::Buffer multiply(const Buffer & left_factor , size_t left_min_index , size_t left_max_index,
                          const Buffer & right_factor, size_t right_min_index, size_t right_max_index)
{
    Buffer product;
    size_t right_index = right_min_index;

    for (; right_index < right_max_index; ++right_index)
    {
        Buffer result;

        if (right_factor[right_index] == 0)
        {
            result.push_back(0);
        }
        else if (right_factor[right_index] == 1)
        {
            result.resize(left_max_index - left_min_index + 1);
            std::copy(left_factor.begin() + left_min_index, left_factor.begin() + left_max_index, result.begin())
        }
        else
        {
            int carry = 0;
            size_t left_index = left_min_index;

            for (; left_index < left_max_index; ++left_index)
            {
                const int res = left_factor[left_index] * right_factor[right_index] + carry;
                carry = res / 10;
                product.push_back(res % 10);
            }

            if (carry > 0)
            {
                product.push_back(carry);
            }
        }
    }

    return product;
}
*/

LongMath LongMath::strassenMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    return left_factor;
}

LongMath LongMath::karatsubaMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
{
    return karatsubaRecursive(left_factor, 0, left_factor.value.size() - 1, right_factor, 0, right_factor.value.size() - 1);   
}

LongMath LongMath::karatsubaRecursive(const LongMath & left_factor , size_t left_min_index , size_t left_max_index,
                                      const LongMath & right_factor, size_t right_min_index, size_t right_max_index) const
{
    const size_t left_size = left_max_index - left_min_index + 1;
    const size_t right_size = right_max_index - right_min_index + 1;

    if (left_size < 2) // FIX
    {
        LongMath::Buffer buffer(right_factor.value.begin() + right_min_index, right_factor.value.begin() + right_max_index);
        multiply(buffer, left_factor.value[left_min_index]);

        LongMath result;
        std::swap(result.value, buffer);
        return result;
    }
    else if (right_size < 2)
    {
        LongMath::Buffer buffer(left_factor.value.begin() + left_min_index, left_factor.value.begin() + left_max_index);
        multiply(buffer, right_factor.value[right_min_index]);

        LongMath result;
        std::swap(result.value, buffer);
        return result;
    }

    const size_t middle_point = std::min(left_size, right_size) / 2 - 1;

    LongMath z0 = karatsubaRecursive(left_factor , left_min_index , middle_point,
                                     right_factor, right_min_index, middle_point);

    const LongMath left_sub_factor  =  left_factor.splitAndSum(left_min_index, left_min_index + middle_point, left_max_index);
    const LongMath right_sub_factor = right_factor.splitAndSum(right_min_index, right_min_index + middle_point, right_max_index);

    const LongMath z1 = karatsubaRecursive(left_sub_factor , 0, left_sub_factor.value.size() - 1,
                                           right_sub_factor, 0, right_sub_factor.value.size() - 1);

    LongMath z2 = karatsubaRecursive(left_factor , middle_point, left_max_index,
                                     right_factor, middle_point, right_max_index);


    z2.opposite();
    z0.opposite();

    LongMath kara_point = z1 + z2 + z0;

    z2.opposite();
    z0.opposite();
    return (z2.shift(2 * middle_point + 1) + kara_point.shift(middle_point + 1) + z0);
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

    for (auto i = 0; i < splitted_size || carry > 0; ++i)
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