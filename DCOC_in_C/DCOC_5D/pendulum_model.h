//
//  pendulum_model.h
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#ifndef pendulum_model_h
#define pendulum_model_h
#include "globals.h"
#include <stdio.h>
void pendulum_nonlinearmodel_ss(double X[num_states], double U, double F, double x_new[num_states],int lol);
#endif /* pendulum_model_h */
