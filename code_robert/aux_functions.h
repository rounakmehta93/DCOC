#include <stdbool.h>

float *linspace(float x0, float x1, int n);
float *linspace2(float x1, float x2, int n_0, int n_1);
void fun_discrete(float *x, float u1);
bool setH_check(float *x);
bool check_notin_G(float *x);
void init_value_function();
int value_iteration();
void write2file();

float max_routine(float *x, int i, int j, int o, int k);
float max_en();

void parallel(int i);

void compute_control(float *x, float *u1);
void simulate(float T, float x1_0, float x2_0, float x3_0, float x4_0);
void write2file();

