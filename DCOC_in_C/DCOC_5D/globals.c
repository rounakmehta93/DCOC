//
//  globals.c
//  DCOC_in_C
//
//  Created by Rounak on 7/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
// Doubts: How to make arrays global.

#include "globals.h"

double prob_matrix_cdf[PROB_MATRIX_LEN][PROB_MATRIX_LEN];
double prob_matrix[PROB_MATRIX_LEN][PROB_MATRIX_LEN] = {{1}};/*
                                                        {{0.2, 0.1, 0.4, 0.2, 0.1}, 
                                                        {0.1, 0.2, 0.3, 0.2, 0.2}, 
                                                        {0.1, 0.2, 0.2, 0.3, 0.2}, 
                                                        {0.2, 0.2, 0.2,0.2, 0.2},
                                                        {0.3, 0.1, 0.3,0.1, 0.2}};*/
double del_t = 0.002;
double u_space[u_len] = {-5,-2.5,0,2.5,5};
double w_space[w_len] = {5.1};//{-5.2,-2.6,0,2.6,5.2};//needs to be equally spaced //needs to have at least 2 elements for interpolation

double x1_space[x1_len],x2_space[x2_len],x3_space[x3_len];
double x0_space[x0_len];
double x_min[] = {-0.32,-1,-0.43,-1.05}; //25 deg and 60 deg/sec
double x_max[] = {0.32,1,0.43,1.05};
double step_size[num_states];
double stopping_criterion = 0.01;


double V[x0_len][x1_len][x2_len][x3_len][w_len];
double V_old[x0_len][x1_len][x2_len][x3_len][w_len];
//double V_load[x0_len][x1_len][x2_len][x3_len][w_len];



void init_value_iteration(){
    step_size[0] = (x_max[0]-x_min[0]) / (x0_len - 1);
    step_size[1] = (x_max[1]-x_min[1]) / (x1_len - 1);
    step_size[2] = (x_max[2]-x_min[2]) / (x2_len - 1);
    step_size[3] = (x_max[3]-x_min[3]) / (x3_len - 1);
    for(int i=0;i<x0_len;i++){
        x0_space[i] = x_min[0] + step_size[0]*i;
        //printf("x0_space %lf\n", x0_space[i]);
    }

    for(int i=0;i<x1_len;i++){
        x1_space[i] = x_min[1] + step_size[1]*i;
        //printf("x1_space %lf\n", x1_space[i]);
    }
    for(int i=0;i<x2_len;i++){
        x2_space[i] = x_min[2] + step_size[2]*i;
        //printf("x2_space %lf\n", x2_space[i]);
    }
    for(int i=0;i<x3_len;i++){
        x3_space[i] = x_min[3] + step_size[3]*i;
        //printf("x3_space %lf\n", x3_space[i]);
    }
    
    
    
    
    //printf("%d init w_len\n",w_len);
    //printf("%d x1_len",x1_len);
}