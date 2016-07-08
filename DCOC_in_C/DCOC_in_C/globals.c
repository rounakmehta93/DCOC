//
//  globals.c
//  DCOC_in_C
//
//  Created by Rounak on 7/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#include "globals.h"
int x1_len;
int x2_len, w_len, u_len;
float prob_matrix[3][3] = {{0.5, 0.1, 0.4}, {0.1, 0.2, 0.7}, {0.3, 0.3, 0.4}};
float del_t = 0.01;
float u_space[] = {-0.01,0,0.01};
float w_space[] = {-0.1,0,0.1};
float x_min[] = {-PI/8,-1};
float x_max[] = {PI/8,1};
float step_size[] = {PI/20,0.1};
float stopping_criterion = 0.01;
int num_states = 2;




void init_value_iteration(){
    x1_len = (x_max[0]-x_min[0])/step_size[0] + 1;
    x2_len = (x_max[1]-x_min[1])/step_size[1] + 1;
    w_len = sizeof(w_space)/sizeof(float);
    u_len = sizeof(u_space)/sizeof(float);
    
    printf("%d x1_len",x1_len);
}