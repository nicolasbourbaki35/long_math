#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include <complex>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <bitset>
#include <stdexcept>

/*
 * Not very effective bit reverse algorithm. Needed by iterative FFT.
 * TODO: replace with bitwise operations
 */
inline size_t bit_reverse(size_t i, size_t n)
{
    if (i>=n) { throw std::out_of_range("Number to reverse >= n which is the max possible value"); }
    if (i==0) return i;
    
    std::bitset<sizeof(size_t)> b(i);
    size_t N = ceil(log2(n));
    
    for(size_t i = 0; i<N/2; ++i)
    {
        bool tmp_bit = b[i];
        b[i] = b[N-i-1];     
        b[N-i-1] = tmp_bit;
    }     

    return b.to_ulong();
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
    
        for (size_t i=0; i<N; ++i)
            A[i] = input[bit_reverse(i,N)];
        
        for (size_t s=1; s<=log2(N); ++s)
        {
            size_t m = 1 << s; 

            double theta =  (inverse ? -1 : 1) * 2 * M_PI / m;
            Complex omega_m = Complex(cos(theta), sin(theta));
            
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

