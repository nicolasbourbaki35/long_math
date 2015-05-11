#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include <complex>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <bitset>
#include <stdexcept>

static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

inline uint32_t bit_reverse(uint32_t i, uint8_t n)
{
    uint32_t c =    (BitReverseTable256[ i        & 0xff] << 24) | 
                    (BitReverseTable256[(i >> 8)  & 0xff] << 16) | 
                    (BitReverseTable256[(i >> 16) & 0xff] << 8 ) |
                    (BitReverseTable256[(i >> 24) & 0xff]);

    c >>= 32 - n;
    return c;
}

/*
 *
 */
template<typename CoefType>
class Polynomial
{
public:
    using CoefVector    = std::vector<CoefType>;
    using CoefVectorIt  = typename CoefVector::const_iterator;
    using Complex       = std::complex<double>;
    using ComplexVector = std::vector<Complex>;
    //to be compatible with std containers
    using value_type    = CoefType;

    Polynomial(std::initializer_list<CoefType> l) 
        : m_coef(l) 
    {}

    Polynomial(CoefVector const & coef) 
        : m_coef(coef) 
    {}

    Polynomial(Polynomial const & p) 
        : m_coef(p.m_coef)   
    {}
    
    Polynomial() {}

    void operator=(Polynomial & p)
    {
        m_coef = p.m_coef;
    }

    Polynomial(Polynomial && p)
    {
        m_coef = p.m_coef;
        p.m_coef.clear();
    }

    template<typename T>
    void assign(std::vector<T> const & coefs)
    {
        m_coef.clear();
        std::copy(coefs.begin(), coefs.end(), std::back_inserter(m_coef));
    }

    size_t size() const   { return m_coef.size(); }
    void resize(size_t n) { m_coef.resize(n);     } 
    
    CoefType  const & operator[](size_t index) const { return m_coef.at(index); } 

    // Multiply by -1
    void negate()
    {
        std::for_each(m_coef.begin(), m_coef.end(), [] (CoefType & c) { c=-c; } );
    }

    template<typename T>
    T operator()(T const & x) const
    {
        return compute_recursive(x, m_coef.begin());
    }

    void operator+=(const Polynomial & p)
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

    Polynomial operator+ (const Polynomial & p) const
    {
        Polynomial tmp(*this);
        tmp+=p; 
        return tmp;
    }

    void operator-=(const Polynomial & p)
    {
        Polynomial tmp(p);
        tmp.negate();
        *this+=tmp;
    }

    Polynomial operator- (const Polynomial & p) const
    {
        Polynomial tmp(p);
        tmp.negate();
        tmp+=*this;

        return tmp;
    }

    void operator*=(const Polynomial & p)
    {
        /*
         * If resulted polynomial size is less than 1500, the naive method performs better
         * 1500 comes from the impirical measurements. See examples/poly_mult_perf
         */
        if (p.m_coef.size() + m_coef.size() - 1 < 1500 )
            naive_multiplication(p);
        else
            FFT_multiplication(p);
    }
    
    Polynomial operator* (const Polynomial & p) const
    {
        Polynomial tmp(*this);
        tmp*=p;
        return tmp;
    }

    bool operator==(const Polynomial & p) const
    {
        return std::equal(m_coef.cbegin(), m_coef.cend(), p.m_coef.cbegin());
    }
 
    bool operator!=(const Polynomial & p) const
    {
        return !(*this==p);
    }
    
    template<typename T>
    friend std::ostream & operator<<(std::ostream &, Polynomial<T> const &);

    /*
     * Naive polynomial multiplication O(N^2)
     */
    void naive_multiplication(const Polynomial<CoefType> & p)
    {
        CoefVector tmp_coefs;
    
        size_t l_size = m_coef.size();
        size_t r_size = p.m_coef.size();
    
        for(size_t i=0; i<l_size+r_size-1; ++i)
        {  
            double coef = 0;
            
            for (size_t j=0; j<i+1; ++j)
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

    /*
     * Multiplication based on Fast Fourier transfomation O(N*log(N))
     */
    void FFT_multiplication(const Polynomial<CoefType> & p)
    {
        size_t result_size = m_coef.size() + p.size();
        
        // Resize in order to have 2^k points 
        size_t n = 1;
        while (n < result_size)
            n = n << 1;
 
        ComplexVector input1 = get_coef_as_complex();
        ComplexVector input2 = p.get_coef_as_complex();
        
        input1.resize(n);
        input2.resize(n);

        ComplexVector fft1 = FFT(input1);
        ComplexVector fft2 = FFT(input2);

        // pointwse multiplication
        for(auto i = 0u; i<n; ++i)
        {
            fft1[i] *= fft2[i];
        }
        
        // inverse FFT
        fft2 = FFT(fft1, true);
        
        m_coef.clear();
        for(auto c : fft2)
        {
            m_coef.push_back(c.real());
        }

        // cut off irrelevant coeficients
        m_coef.resize(result_size - 1);
    }


private:
    
    /*
     * returns coeficients as a vector of complex numbers
     */
    ComplexVector get_coef_as_complex() const
    {
        ComplexVector v;
        for(auto c : m_coef) 
            v.push_back(Complex(c,0));
        return v;
    }

    /*
     * Horner's recursive method for calculating polynomial
     */
    template<typename T>
    T compute_recursive(T const & x, CoefVectorIt it) const
    {
	    if (it == m_coef.end())
            return T(0);
	    return *it + x * compute_recursive(x, ++it);
    }
    
    /*
     * Horner's recursive method for calculating polynomial. 
     * Specialization for complex numbers.
     */
    template<typename T>
    std::complex<T> compute_recursive (std::complex<T> const & x, CoefVectorIt it) const
    {
        if (it == m_coef.end())
            return std::complex<T>(0, 0);

        auto res = std::complex<T>(*it, 0);
        res +=  x * compute_recursive(x, ++it);
        return res;
    }

    ComplexVector FFT(ComplexVector const & input, bool inverse = false)
    {
        size_t N = input.size();
        ComplexVector A;
        A.resize(N);
   
        size_t N_l = ceil(log2(N));
        for (size_t i=0; i<N; ++i)
            A[i] = input[bit_reverse(i,N_l)];
        
        for (size_t s=1; s<=log2(N); ++s)
        {
            size_t m = 1 << s; 

            double theta =  (inverse ? -1 : 1) * 2 * M_PI / m;
            double sint,cost;
            sincos(theta, &sint, &cost);
            Complex omega_m = Complex(cost, sint);
            
            for (size_t k=0; k<N; k+=m)
            {
                Complex omega = 1;
                for(size_t j=0; j<m/2; ++j)
                {
                    Complex t = omega * A[k+j+m/2];         
                    Complex u = A[k+j];
                    A[k+j]     = u + t;
                    A[k+j+m/2] = u - t;
                    omega *= omega_m;
                }
            }
        }

        if (inverse)
        {
            std::for_each(A.begin(), A.end(), [&] (Complex & c) { c/=N; } );
        }
        
        return A;
    }

private:
    CoefVector m_coef;
};

template<typename T>
std::ostream & operator<<(std::ostream & os, Polynomial<T> const & poli)
{
    size_t pow = poli.size()-1;
    
    for(auto it = poli.m_coef.rbegin(); it!=poli.m_coef.rend(); ++it, --pow)
    {
        if (*it == 0)
           continue;

        if (*it > 0 && it != poli.m_coef.rbegin())
        {
            os << "+";
        }
        
        os << (double)*it;
        
        if (pow != 0)
        {
            os << "*x^" << pow;
        }
    }
    return os;
}

#endif

