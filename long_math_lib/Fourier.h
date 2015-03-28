#include <complex>
#include <vector>

namespace Fourier
{
    template<typename Function, typename T>
    std::vector<std::complex<T> > fourier(Function const & f, size_t p)
    {
        std::vector<std::complex<T> > res;
        for(size_t i=0; i<p; ++i)
        {
            std::complex<T> omega = std::exp(std::complex<T>(0, 2 * M_PI * i / p));
            res.push_back(f(omega)); 
        }
        return res;
    }

};

