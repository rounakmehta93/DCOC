//
//  pendulum_model.h
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#ifndef pendulum_model_h
#define pendulum_model_h

#include <stdio.h>
extern int num_states;
void pendulum_nonlinearmodel_ss1(float X[num_states], float U, float F, float x_new[num_states]);
#endif /* pendulum_model_h */
