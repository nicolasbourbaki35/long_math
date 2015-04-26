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

    LongMath() : sign(Sign::POS)
    {}

    LongMath(Buffer const & buf, Sign const & s = Sign::POS) 
        : value(buf)
        , sign(s)
    {}

    LongMath(Buffer::const_iterator begin, Buffer::const_iterator end, Sign const & s = Sign::POS) 
        : value(begin, end)
        , sign(s)
    {}

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
        if (isZero() && long_math.isZero())
            return true; 
        
        if (sign != long_math.sign || value.size() != long_math.value.size())
            return false;

        const auto res = std::mismatch(value.begin(), value.end(), long_math.value.begin());
        return (res.first == value.end() && res.second == long_math.value.end());
    }

    bool isZero() const 
    {
        for(auto d : value)
        {
            if (d!=0)
                return false;
        }
        return true;       
    }

    bool operator== (const int & other_value) const
    {
        LongMath other(other_value);    
        return (*this) == other;
    }

    LongMath operator+ (const LongMath & lm) const;
    LongMath operator- (const LongMath & lm) const;
    LongMath operator* (const LongMath & right_factor) const;

    bool isNegative() const { return sign == Sign::NEG; }

    void opposite() { sign = (sign == Sign::NEG) ? Sign::POS : Sign::NEG; }

    friend std::ostream & operator<<(std::ostream &, LongMath const &);

    LongMath & shift(int power);
    LongMath splitAndSum(size_t min_index, size_t index, size_t max_index) const;

    LongMath operator* (int right_factor) const;
    void strassenMultiplication (const LongMath & right_factor);
    void karatsubaMultiplication(const LongMath & right_factor);
    void standardMultiplication (const LongMath & right_factor);

private:
    LongMath karatsubaRecursive(const LongMath & left_factor, const LongMath & right_factor);

    void setFromInt(int val);
    void setFromString(std::string const & val);

private:
    static const size_t TRIGGER_STRASSEN = 100;
    static const size_t TRIGGER_KARATSUBA = 10;

    Buffer value;
    Sign   sign;
};
    
std::ostream & operator<<(std::ostream & os, LongMath const & lm);

