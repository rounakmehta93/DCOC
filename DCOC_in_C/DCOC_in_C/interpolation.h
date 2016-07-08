//
//  interpolation.h
//  DCOC_in_C
//
//  Created by Rounak on 7/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#ifndef interpolation_h
#define interpolation_h

#include <stdio.h>
extern int x1_len, x2_len, w_len, u_len;
int find_nearest_neighbor(float val, float step_size, float min, float max);
float interpol_1D(float v0, float v1, float x0, float val, float step_size);
float interpol_3D(float x1_space[x1_len], float x2_space[x2_len], float w_space[w_len], float V[x1_len][x2_len][w_len], float x1,float  x2, float w);

#endif /* interpolation_h */
