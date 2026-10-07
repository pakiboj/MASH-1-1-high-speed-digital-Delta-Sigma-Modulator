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
    int n{}; n = number_of_data("Podaci");

    double* p_data{new double[n] {}};
    int* p_q{new int[n] {}};

    ac_fixed<32,5,true,AC_TRN,AC_SAT> x1, x1d=0.0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> x2, x2d=0.0, hx2, hx2d=0.0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> x3, x3d=0.0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> x4, x4d=0.0, hx4, hx4d=0.0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> x5, x5d=0.0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> y, vout=0;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> uin;
    ac_fixed<32,5,true,AC_TRN,AC_SAT> fb=0.0;
    
    load_data("Podaci", p_data, n);
    
    for(size_t i=0; i<n; i++){
        uin=p_data[i]; 
        fb = 0.0;
        fb = fb - vout;
        x1   = x1d     + ((uin+fb)>>6)+((uin+fb)>>8); 
        x2   = x2d     + hx2d;
        x3   = x3d     + x2-(x2>>3)-(x2>>6)    + (fb>>4)-(fb>>9)+(fb>>11);
        hx2  = -((x3>>4)-(x3>>6)+(x3>>8))     + (fb>>5)-(fb>>8)-(fb>>11)    + (x1>>1)-(x1>>3)+(x1>>6);
        x4   = x4d     +    hx4d; 
        x5   = x5d     + (x4<<1)+(x4>>1)-(x4>>4)    + (fb>>3)+(fb>>6)-(fb>>9);
        hx4  = -((x5>>4)-(x5>>6)+(x5>>8))      + (fb>>5)+(fb>>7)        + (x3)-(x3>>2)-(x3>>4);
        y    = (x5<<4)+(x5<<3)-x5;

        // u jupyteru su prave brojke, ovo pomeranje treba da bude sto preciznije za g parametre, 

        x1d=x1; 
        hx2d=hx2; 
        x2d=x2; 
        x3d=x3; 
        hx4d=hx4; 
        x4d=x4; 
        x5d=x5;
        vout=quantizer(y); 
        p_q[i]=vout.to_int();
    }
    write_data("Obradjeni_real", p_q, n);
    delete[] p_data; 
    p_data=nullptr; 
    delete[] p_q; 
    p_q=nullptr; 
    return 0;
}
