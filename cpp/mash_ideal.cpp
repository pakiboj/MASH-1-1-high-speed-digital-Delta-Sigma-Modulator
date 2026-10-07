#include <iostream>
#include "DSM_IO.hpp"
#include <cmath>


int quantizer(double in){
    int out = (int)floor(in);
    if (in <= -15) out = -15;
    else if (in >= 15) out = 15;
    else {
        if ((out % 2) == 0){out++;}}
    return out;
}
int main(){
    double uin, y, vout=0;
    double w1, w1d=0, y1, y1d=0, y1dd=0, e1=0, v1=0;
    double w2, w2d=0, y2, y2d=0, e2=0, v2=0, v2d=0;

    
    int n{}; n = number_of_data("Podaci");

    double* p_data{new double[n] {}};
    int* p_q{new int[n] {}};
    
    load_data("Podaci", p_data, n);
    int i;
    for(size_t i=0; i<n; i++){
        uin=p_data[i]; 
        vout = v1 + v2d;

        y1 = quantizer(w1d);
        e1 = (w1d - y1);
        w1 = uin + e1;

// -----------------------------------

        y2 = quantizer(w2d); 
        e2 = (w2d - y2);
        w2 = e1 + e2;

        v2 = y2- y2d;

// -----------------------------------

        v1 = y1d;
        y1d = y1;
        w1d = w1;
        w2d = w2;
        y2d = y2;
        v2d = v2;

        
        p_q[i]=vout;
    }
    write_data("Obradjeni_mash", p_q, n);
    delete[] p_data; 
    p_data=nullptr; 
    delete[] p_q; 
    p_q=nullptr; 
    return 0;
}
// g++ -O2 -std=c++17 -o dsm mash_ideal.cpp DSM_IO.hpp
// ./dsm