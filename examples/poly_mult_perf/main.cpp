#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "Polynomial.h" 

using namespace std;

#define MAX_ITER 5000u

int main(int  argc, char ** argv)
{
    vector<double> v;
    
    for(auto i=1u; i<=MAX_ITER; i+=50)
    {
        std::cout << 2*i  << "\t" ;
        
        // init coeficients randomly
        v.clear();
        
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(-100, 100);
        for(auto j=0u; j<i; ++j)
        {
            v.push_back(dis(gen));
        }

        Polynomial<double> p1(v), p2(v), p3(v);
       
        { 
            auto s = chrono::high_resolution_clock::now();
            
            p1.naive_multiplication(p3);
            
            auto e = chrono::high_resolution_clock::now();
            
            cout << chrono::duration_cast<chrono::microseconds>(e - s).count() << "\t";
        }

        { 
            auto s = chrono::high_resolution_clock::now();
            
            p2.FFT_multiplication(p3);
            
            auto e = chrono::high_resolution_clock::now();
            
            cout << chrono::duration_cast<chrono::microseconds>(e - s).count() << endl;
        }
    }
}

