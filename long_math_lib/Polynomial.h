#include <complex>
#include <vector>
#include <initializer_list>

class Polynomial
{
public:
    using CoefVector    = std::vector<double>;
    using CoefVectorIt  = CoefVector::iterator;

    Polynomial(std::initializer_list<double> l) 
        : m_coef(l) 
    {}

    Polynomial(CoefVector const & coef) 
        : m_coef(coef) 
    {}

    Polynomial(Polynomial const & p) 
        : m_coef(p.m_coef)   
    {}

    void operator=(Polynomial & p)
    {
	    m_coef = p.m_coef;
    }

    Polynomial(Polynomial && p)
    {
	    m_coef = p.m_coef;
        p.m_coef.clear();
    }

    size_t degree() const { return m_coef.size() - 1; }

    /*
     * Multiply by -1
     */
    void negate();

    template<typename T>
    T operator()(T const & x)
    {
        return compute_recursive(x, m_coef.begin());
    }

    void       operator+=(const Polynomial & p);
    Polynomial operator+ (const Polynomial & p) const;
    void       operator-=(const Polynomial & p);
    Polynomial operator- (const Polynomial & p) const;
    void       operator*=(const Polynomial & p);
    Polynomial operator* (const Polynomial & p) const;
    bool       operator==(const Polynomial & p) const;

    friend std::ostream & operator<<(std::ostream &, Polynomial const &);

private:
    
    /*
     * Horner's recursive method for calculating polynomial
     */
    template<typename T>
    T compute_recursive(T const & x, CoefVectorIt it)
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
    std::complex<T> compute_recursive (std::complex<T> const & x, CoefVectorIt it) 
    {
        if (it == m_coef.end())
            return std::complex<T>(0, 0);

        auto res = std::complex<T>(*it, 0);
        res +=  x * compute_recursive(x, ++it);
        return res;
    }

    /*
     * Naive polynomial multiplication O(N^2)
     */
    void naive_multiplication(const Polynomial & p);

private:
    CoefVector m_coef;
};

std::ostream & operator<<(std::ostream & os, Polynomial const & poli);

