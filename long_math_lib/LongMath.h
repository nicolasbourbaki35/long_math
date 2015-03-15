#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

class LongMath
{
public:
    typedef std::vector<char> Buffer;
    typedef Buffer::iterator BufferIt;    
    typedef Buffer::const_iterator BufferConstIt;
    
    enum class Sign : char { POS = '+', NEG='-' };

    LongMath() 
    : sign(Sign::POS)
    {
    }

    LongMath(std::string const & val)
    {
        setFromString(val);
    }

    LongMath(int val)
    {
        setFromInt(val);
    }

    bool operator== (const LongMath & long_math) const
    {    
        if (sign != long_math.sign)
            return false;

        const auto res = std::mismatch(value.begin(), value.end(), long_math.value.begin());
        return (res.first == value.end() && res.second == long_math.value.end());
    }

    bool operator== (const int & other_value) const
    {
        LongMath other(other_value);    
        return (*this) == other;
    }

    LongMath operator+ (const LongMath & lm) const
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

    LongMath operator* (int right_factor) const
    {
        const LongMath left_factor(*this);
        
        // TODO

        return left_factor;   
    }

    LongMath operator* (const LongMath & right_factor) const
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

    LongMath strassenMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
    {
        return left_factor;
    }

    LongMath karatsubaMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
    {
        if (left_factor.value.size() < 2 || right_factor.value.size() < 2)
            return standardMultiplication(left_factor, right_factor);

        const size_t middle_point = min(left_factor.value.size(), right_factor.value.size()) / 2;

        

    }

    LongMath standardMultiplication(const LongMath & left_factor, const LongMath & right_factor) const
    {
        LongMath result(0);

        unsigned int index = 0;

        for (; index < left_factor.size(); ++index)
        {
            LongMath tmp = right_factor * left_factor[index];
            tmp.shift(index);

            result = result + tmp;
        }

        return result;
    }

    LongMath & shift(int power)
    {
        // TODO
        return *this;
    }

    LongMath splitAt(size_t index) const
    {
        LongMath splitted(*this);

        // TODO
        return splitted;
    }


    bool isNegative() const { return sign == Sign::NEG; }

    friend std::ostream & operator<<(std::ostream &, LongMath const &);

private:
    void setFromInt(int val)
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

    void setFromString(std::string const & val)
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

private:
    Buffer value;
    Sign   sign;
};
    
std::ostream & operator<<(std::ostream & os, LongMath const & lm);

