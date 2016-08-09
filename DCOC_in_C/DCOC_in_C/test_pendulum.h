//
//  test_pendulum.h
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#ifndef test_pendulum_h
#define test_pendulum_h

#include <stdio.h>
extern int x1_len, x2_len, w_len, u_len, num_states, arr_len;
void test_pendulum();
void load_V(float V[x1_len][x2_len][w_len]);
float get_u_value_iteration(float V[x1_len][x2_len][w_len],float x[num_states],float w, float x1_space[x1_len], float x2_space[x2_len], float *V_in_arr);
int indexOfw(float w);
float markov_model(float w);
int writeToFileXU(float x_array[2][arr_len], float u_array[arr_len], float t_array[arr_len],float V_array[arr_len],int exit_i);
#endif /* test_pendulum_h */
