#include <iostream>
#include "DSM_IO.hpp"
#include "ac_fixed.h"


ac_fixed<5, 5, true, AC_TRN, AC_SAT> quantizer(ac_fixed<5, 5, true, AC_TRN, AC_SAT> in){
    ac_fixed<5, 5, true, AC_TRN, AC_SAT> out = in;
    if (in <= -15) out = -15;
    else if (in >= 15) out = 15;
    else {
        if ((in & 1) == 0) out = in + 1;
    }
    return out;
}

int main(){
    ac_fixed<16, 5, true, AC_TRN, AC_SAT> uin;
    ac_fixed<5, 5, true, AC_TRN, AC_SAT> vout=0;
    ac_fixed<12, 5, true, AC_TRN, AC_SAT> w1, w1d=0;
    ac_fixed<16, 1, true, AC_TRN, AC_SAT> e1=0;
    ac_fixed<5, 5, true, AC_TRN, AC_SAT> y1, v1=0, y1d=0;
    ac_fixed<12, 5, true, AC_TRN, AC_SAT> w2, w2d=0;
    ac_fixed<12, 1, true, AC_TRN, AC_SAT> e2=0;
    ac_fixed<5, 5, true, AC_TRN, AC_SAT> y2, y2d=0, v2=0, v2d=0;
    
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

        y2 = quantizer(w2d); 
        e2 = (w2d - y2);
        w2 = e1 + e2;

        v2 = y2- y2d;


        v1 = y1d;
        y1d = y1;
        w1d = w1;
        w2d = w2;
        y2d = y2;
        v2d = v2;

        p_q[i]=vout.to_int();

    }
    write_data("Obradjeni_mash_real", p_q, n);
    delete[] p_data; 
    p_data=nullptr; 
    delete[] p_q; 
    p_q=nullptr; 
    return 0;
}
// g++ -O2 -std=c++17 -I ../../ac_types/include -o dsm_real mash_real.cpp
// ./dsm_real

// g++ -O2 -std=c++17 -I ../../ac_types/include -o dsm_real mash_real.cpp && ./dsm_real