#include <stdio.h>
#include <math.h>
#include "aux_functions.h"
#include "globals.h"

int main(){

  //x1_grid = linspace2(0.03,x1_max,19,13); 
  //x2_grid = linspace2(0.05,x2_max,19,13);
  //x3_grid = linspace2(0.028,x3_max,19,13);
  //x4_grid = linspace2(0.06,x4_max,19,13);

  x1_grid = linspace(x1_min,x1_max,n1); 
  x2_grid = linspace(x2_min,x2_max,n2);
  x3_grid = linspace(x3_min,x3_max,n3);
  x4_grid = linspace(x4_min,x4_max,n4);

  U1 = linspace(-6.0,6.0,nn1);
  
  L = (m_p*l)*(m_p*l)/(I+m_p*l*l);
  J_eq = m_c +eta_g*K_g*K_g*J_m/r_mp/r_mp; 
  F_const = eta_g*K_g*eta_m*K_t/(r_mp*r_mp*R_m);

    printf("starting initialization of V\n");
    init_value_function();
    printf("end of initialization of V\n");
    value_iteration();
    write2file();
    simulate(12.0, 0.0, 0.0, 0.0, 0.0);
  
  return 0;
}

