#include "LongMath.h"

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

