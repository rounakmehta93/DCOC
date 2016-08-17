#ifndef value_iteration_h
#define value_iteration_h
#include "globals.h"

#include <stdio.h>
extern int arr_len;
double max_of_V();
void deepcopy();
int writeToFile();
void parallel(int i0);
int value_iteration();
#endif /* value_iteration_h */