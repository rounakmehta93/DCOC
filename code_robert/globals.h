 #include <stdbool.h>

#define dT 0.01

#define NTHREADS 8

#define  n1 25//49
#define  n2 25//49
#define  n3 25//49
#define  n4 25//49

#define eta_g 0.9
#define eta_m 0.69
#define K_g 3.71
#define K_t 0.00768
#define K_m 0.00768
#define r_mp 0.00635
#define R_m 2.6
#define m_c 0.75
#define m_p 0.127
#define I 0.0012
#define B_eq_nominal 5.4
#define B_p_nominal 0.0024
#define l 0.1778
#define g 9.81
#define J_m 0.00000039


#define Fw_cart 0.03 // Force acting on cart 
#define Fw_p 0.1 // Force acting on pendulum
#define l_Fw_p 0.1778 // lever for Fw_p

#define x1_min -0.32
#define x1_max 0.32
#define x2_min -0.8
#define x2_max 0.8
#define x3_min -0.2//-1.5708
#define x3_max 0.2//1.5708
#define x4_min -1.2
#define x4_max 1.2

#define nn1 21//101//41

float K[n1][n2][n3][n4];
float V[n1][n2][n3][n4];
float en[n1][n2][n3][n4];
bool FLG[n1][n2][n3][n4];
float *x1_grid,*x2_grid,*x3_grid,*x4_grid, *U1;
float x1_step, x2_step, x3_step, x4_step;

float L;
float J_eq;
float F_const;
float base_yield;
