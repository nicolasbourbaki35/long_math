#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

class LongMath
{
public:
    typedef std::vector<char> Buffer;
    typedef Buffer::iterator BufferIt;    
    typedef Buffer::reverse_iterator BufferReverseIt;    
    typedef Buffer::const_iterator BufferConstIt;
    
    enum class Sign : char { POS = '+', NEG='-' };

    LongMath() 
        :/* value(1, 0)
        , */sign(Sign::POS)
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

    LongMath(int64_t val)
    {
        setFromInt(val);
    }
    
    void setSign(Sign const & s ) { sign = s;    }
    Sign const & getSign() const  { return sign; }

    bool operator== (const LongMath & lm) const
    {   
        return compare(lm) == 0;       
    }

    bool operator== (const int & i) const
    {
        LongMath other(i);    
        return (*this) == other;
    }
    
    bool operator!= (const LongMath & lm) const
    {
        return compare(lm) != 0;
    }

    bool operator< (const LongMath & lm) const;
    bool operator> (const LongMath & lm) const;
 
    LongMath operator+ (const LongMath & lm) const;
    LongMath operator- (const LongMath & lm) const;
    LongMath operator* (const LongMath & right_factor) const;
   
    int8_t compare(const LongMath & lm) const;
    int8_t absCompare(const LongMath & lm) const;

    bool isNegative() const { return sign == Sign::NEG; }
    bool isZero() const
    { 
        auto it =  std::find_if(value.begin(), value.end(), [](char const & d) { return d != 0; });
        return it == value.end();
    }

    void opposite() { sign = (sign == Sign::NEG) ? Sign::POS : Sign::NEG; }

    friend std::ostream & operator<<(std::ostream &, LongMath const &);

    LongMath operator<<(int power) const;

    LongMath operator* (int right_factor) const;
    void strassenMultiplication (const LongMath & right_factor);
    void karatsubaMultiplication(const LongMath & right_factor);
    void standardMultiplication (const LongMath & right_factor);

private:
    LongMath karatsubaRecursive(const LongMath & left_factor, const LongMath & right_factor);

    void setFromInt(int64_t val);
    void setFromString(std::string const & val);

private:
    static const size_t TRIGGER_STRASSEN = 100;
    static const size_t TRIGGER_KARATSUBA = 10;

    Buffer value;
    Sign   sign;
};
    
std::ostream & operator<<(std::ostream & os, LongMath const & lm);

