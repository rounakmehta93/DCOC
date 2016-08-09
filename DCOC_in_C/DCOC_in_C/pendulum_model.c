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

extern float del_t;
extern float u_space[];
extern float w_space[];
extern float x_min[];
extern float x_max[];
extern float step_size[];
extern float stopping_criterion;
extern int num_states;

void pendulum_nonlinearmodel_ss1(float X[num_states], float U, float F, float x_new[num_states]){
    //printf("delt %f",del_t);
    //th is 0 when pendulum is in stable equilibrium. ccw +ve. U : input torque. F: disturbance force

    float m = 0.2;
    float b = 0;//0.001;
    float I = 0.006;
    float g = 9.8;
    float l = 0.3;
    float th = X[0];
    float th_ = X[1];
    float T = U;
    x_new[0] = X[0] + del_t* th_;
    x_new[1] = X[1] + del_t* (-m*g*l*sin(th) -b*th_ + F*l*cos(th) + T)/I; //cos and sin in radians
    //printf("%f x in function \n",x_new[1]);
    
    
}
