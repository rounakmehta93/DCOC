//
//  test_pendulum.h
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#ifndef test_pendulum_h
#define test_pendulum_h
#include "globals.h"

#include <stdio.h>
extern int arr_len;
void test_pendulum();
void load_V();
double get_u_value_iteration(double x[num_states],double w);
int indexOfw(double w);
double markov_model(double w);
int writeToFileXU(double x_array[2][arr_len], double u_array[arr_len], double w_array[arr_len], double t_array[arr_len], int exit_i);
#endif /* test_pendulum_h */
