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

int find_nearest_neighbor(double val, double step_size, double min, double max);
double interpol_1D(double v0, double v1, double x0, double val, double step_size);
double interpol_5D(double x0, double x1,double  x2, double  x3, double w);
double interpol_4D(double x0, double x1,double  x2, double  x3, double w);
#endif /* interpolation_h */
