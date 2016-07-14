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


void pendulum_nonlinearmodel_ss(double X[num_states], double U, double v, double x_new[num_states]){
    //printf("delt %f",del_t);
    
   
    
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
    double F = U;
    
    x_new[0] = X[0] + del_t*x_;
    
    x_new[1] = X[1] + del_t*(-(I*b1*v - F*I + I*b2*v + I*b1*x_ + I*b2*x_ - F*l*l*m + b1*l*l*m*v + b2*l*l*m*v + b1*l*l*m*x_ + b2*l*l*m*x_ - (g*l*l*m*m*sin(2*ph))/2 + l*l*l*m*m*ph_*ph_*sin(ph) + I*l*m*ph_*ph_*sin(ph) - b2*l*l*m*v*cos(ph) - b2*l*l*m*x_*cos(ph))/(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m));
    
    x_new[2] = X[2] + del_t*ph_;
    //printf("x_new[2] %lf", x_new[2]);
    x_new[3] = X[3] + del_t*((l*(2*g*m*m*sin(ph) + 2*M*b2*v + 2*M*b2*x_ + 2*b2*m*v + 2*b2*m*x_ + 2*F*m*cos(ph) + 2*M*g*m*sin(ph) - 2*b1*m*v*cos(ph) - 2*b2*m*v*cos(ph) - 2*b1*m*x_*cos(ph) - 2*b2*m*x_*cos(ph) - l*m*m*ph_*ph_*sin(2*ph)))/(2*(I*m + I*M + l*l*m*m - l*l*m*m*cos(ph)*cos(ph) + M*l*l*m)));
    
    
}
