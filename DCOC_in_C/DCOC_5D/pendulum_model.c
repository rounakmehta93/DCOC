//
//  pendulum_model.c
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#include "pendulum_model.h"
#include <math.h>
#include <stdlib.h>
#include "globals.h"

extern double del_t;
extern double u_space[];
extern double w_space[];
extern double x_min[];
extern double x_max[];
extern double step_size[];
extern double stopping_criterion;


void pendulum_nonlinearmodel_ss(double X[num_states], double V, double dist, double x_new[num_states])
{
    //V is input voltage. dist is diturbance voltage

    double eta_g = 0.9;
    double eta_m = 0.69;
    double K_g = 3.71;
    double K_t = 7.68*0.001;
    double K_m = 7.68*0.001;
    double r_mp = 6.35*0.001;
    double R_m = 2.6;
    double m_c = 0.38;
    double m_p = 0.23;
    double I = 7.88*0.001;
    double B_eq_nominal = 4.3;
    double B_p_nominal = 0.0024;
    double l = 0.3302;
    double g = 9.81;
    double J_m = 3.9*0.0000001; // 10^-7

    double L = (m_p*l)*(m_p*l)/(I+m_p*l*l);
    double M = m_c + m_p;
    double J_eq = m_c + eta_g*K_g*K_g*J_m/r_mp/r_mp;

    double B_eq = B_eq_nominal;
    double B_p = B_p_nominal;
    double F_const = eta_g*K_g*eta_m*K_t/(r_mp*r_mp*R_m);
    double x1 = X[0];
    double x2 = X[1]; // index change since this had been copied from MATLAB.
    double x3 = X[2];
    double x4 = X[3];
    V = V + dist; //for disturbance
    //if(lol==0)
    //printf("%ld \n", V);
    double F = F_const * (V*r_mp-K_m*K_g*x2);
    double term = L*g*sin(x3)*cos(x3) - B_p*L*cos(x3)*x4/m_p/l - m_p*l*sin(x3)*x4*x4 + F - B_eq*x2;
    x_new[0] = X[0] + del_t * x2;
    x_new[1] = X[1] + del_t * 1/(J_eq + m_p - L*cos(x3)*cos(x3))*term;
    x_new[2] = X[2] + del_t * x4;
    x_new[3] = X[3] + del_t * (L*g/m_p/l*sin(x3) - B_p/(I+m_p*l*l)*x4 + (L/m_p/l*cos(x3))/(J_eq+m_p-L*cos(x3)*cos(x3))*term);
}
/*
{
    //printf("delt %f",del_t);
    
   
    //v is disturbance.
    double M = 0.5;
    double m = 0.2;
    double b1 = 0.1;
    double b2 = 0.01;
    double I = 0.006;
    double g = 9.8;
    double l = 0.3;
    
    //double x = X[0];
    double x_ = X[1];
    double ph = X[2];
    double ph_ = X[3];
    double F = V;
    double v = dist;
    
    //printf("x0 %lf x1 %lf x2 %lf  x3 %lf , U %lf  lol %d\n ",X[0],X[1],X[2],X[3], F, lol);
    
    x_new[0] = X[0] + del_t*x_;
    
    //x_new[1] = X[1] + del_t*(-(I*b1*v - F*I + I*b2*v + I*b1*x_ + I*b2*x_ - F*l*l*m + b1*l*l*m*v + b2*l*l*m*v + b1*l*l*m*x_ + b2*l*l*m*x_ - (g*l*l*m*m*sin(2*ph))/2 + l*l*l*m*m*ph_*ph_*sin(ph) + I*l*m*ph_*ph_*sin(ph) - b2*l*l*m*v*cos(ph) - b2*l*l*m*x_*cos(ph))/(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m));
    
    //without distrubance v for debugging
        x_new[1] = X[1] + del_t*(-( - F*I  + I*b1*x_ + I*b2*x_ - F*l*l*m  + b1*l*l*m*x_ + b2*l*l*m*x_ - (g*l*l*m*m*sin(2*ph))/2 + l*l*l*m*m*ph_*ph_*sin(ph) + I*l*m*ph_*ph_*sin(ph)  - b2*l*l*m*x_*cos(ph))/(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m));
    x_new[2] = X[2] + del_t*ph_;
    //printf("x_new[2] %lf", x_new[2]);
    //x_new[3] = X[3] + del_t*((l*(2*g*m*m*sin(ph) + 2*M*b2*v + 2*M*b2*x_ + 2*b2*m*v + 2*b2*m*x_ + 2*F*m*cos(ph) + 2*M*g*m*sin(ph) - 2*b1*m*v*cos(ph) - 2*b2*m*v*cos(ph) - 2*b1*m*x_*cos(ph) - 2*b2*m*x_*cos(ph) - l*m*m*ph_*ph_*sin(2*ph)))/(2*(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m)));
    //without v for debugging
    x_new[3] =  X[3] + del_t*((l*(2*g*m*m*sin(ph) + 2*M*b2*x_ + 2*b2*m*x_ + 2*F*m*cos(ph) + 2*M*g*m*sin(ph) - 2*b1*m*x_*cos(ph) - 2*b2*m*x_*cos(ph) - l*m*m*ph_*ph_*sin(2*ph)))/(2*(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m)));
    //printf("x3 %lf lol %d\n ",X[3], lol+5);
    
}
*/
