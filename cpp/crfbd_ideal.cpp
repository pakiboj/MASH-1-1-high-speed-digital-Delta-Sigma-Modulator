#include <iostream>
#include "DSM_IO.hpp"
#include <cmath>

const double a1 = 0.0201;
const double a2 = 0.0277;
const double a3 = 0.061;
const double a4 = 0.0397;
const double a5 = 0.1386;

const double b1 = a1;

const double g1 = 0.0516;
const double g2 = 0.0516;

const double c1 = 0.3933;
const double c2 = 0.863;
const double c3 = 0.7017;
const double c4 = 2.4262;
const double c5 = 23.0362;


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
    double x1, x1d=0.0;
    double x2, x2d=0.0, hx2, hx2d=0.0;
    double x3, x3d=0.0;
    double x4, x4d=0.0, hx4, hx4d=0.0;
    double x5, x5d=0.0;
    
    int n{}; n = number_of_data("Podaci");

    double* p_data{new double[n] {}};
    int* p_q{new int[n] {}};
    
    load_data("Podaci", p_data, n);
    int i;
    for(size_t i=0; i<n; i++){
        uin=p_data[i]; 
        x1   = x1d     + a1*(uin-vout); 
        x2   = x2d     + hx2d;
        x3   = x3d     + x2*c2    + a3*(-vout);
        hx2  = -x3*g1    + a2*(-vout)    + x1*c1;
        x4   = x4d     +    hx4d; 
        x5   = x5d     + x4*c4    + a5*(-vout);
        hx4  = -x5*g2     + a4*(-vout)        + x3*c3;
        y    = x5*c5; 

        x1d=x1; 
        hx2d=hx2; 
        x2d=x2; 
        x3d=x3; 
        hx4d=hx4; 
        x4d=x4; 
        x5d=x5;
        vout=quantizer(y); 
        p_q[i]=vout;
    }
    write_data("Obradjeni_ideal", p_q, n);
    delete[] p_data; 
    p_data=nullptr; 
    delete[] p_q; 
    p_q=nullptr; 
    return 0;
}
