#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "interpn.h"

float delta = 0.0;

float *linspace(float x0, float x1, int n){
  float step = (x1-x0)/(n-1);
  float *x_grid;
  x_grid = (float *) malloc(n*sizeof(float));
  x_grid[0]=x0;
  for (int i=1;i<n;i++){
    x_grid[i]=x_grid[i-1]+step;}
  return x_grid;
}

float *linspace2(float x1, float x2, int n_0, int n_1){
  int n = 2*n_1+n_0;
  float step1 = (x2-x1)/n_1;
  float step0 = 2*x1/(n_0-1);
  float *x_grid;
  x_grid = (float *) malloc(n*sizeof(float));
  x_grid[0]=-x2;
  for (int i=1;i<n_1+1;i++)
    x_grid[i]=x_grid[i-1]+step1;
  for (int i=n_1+1;i<n_1+n_0;i++)
    x_grid[i]=x_grid[i-1]+step0;
  for (int i=n_1+n_0;i<n;i++)
    x_grid[i]=x_grid[i-1]+step1;
  return x_grid; 
}


void fun_discrete(float *x, float Volts){
  float B_eq = B_eq_nominal;
  float B_p = B_p_nominal;
  float x1 = x[0]; float x2 = x[1]; float x3 = x[2]; float x4=x[3];
  float F = F_const * (Volts*r_mp-K_m*K_g*x2);
  float term = L*g*sin(x3)*cos(x3) - B_p*L*cos(x3)*x4/m_p/l - m_p*l*sin(x3)*x4*x4 + F - B_eq*x2 - Fw_p - Fw_cart;
  float term2 = J_eq + m_p - L*cos(x3)*cos(x3);
  float term3 = I+m_p*l*l;
  x[0] = x1+dT*x2;
  x[1] = x2+dT*(term/term2);
  x[2] = x3+dT*x4;
  x[3] = x4+dT*(L*g/m_p/l*sin(x3) - B_p/term3*x4 + (L/m_p/l*cos(x3))/term2*term + Fw_p*cos(x3)*l_Fw_p/term3);
}

bool check_notin_G(float *x){
  bool check = 0;
  if (x[0]>x1_grid[n1-1] || x[0]<x1_grid[0]){
    check = 1;return check;}
  else if (x[1]>x2_grid[n2-1] || x[1]<x2_grid[0]){
    check = 1;return check;}
  else if (x[2]>x3_grid[n3-1] || x[2]<x3_grid[0]){
    check = 1;return check;}
  else if (x[3]>x4_grid[n4-1] || x[3]<x4_grid[0]){
    check = 1;return check;}
  return check;
}

void init_value_function(){
  for(int i=0;i<n1;i++){
    for(int j=0;j<n2;j++){
      for(int o=0;o<n3;o++){
	for(int k=0;k<n4;k++){ 
	    FLG[i][j][o][k] = 1;
	    V[i][j][o][k] = 1.0;
	    K[i][j][o][k] = 1.0;
	    en[i][j][o][k] = 1.0;
	  }     
      }
    }
  }
}

void max_routine(float *x, int i, int j, int o, int k){
  float yield = 1.0;
  float V_u=0.0;
  float x0[4];
  x0[0]=x[0]; x0[1]=x[1]; x0[2]=x[2]; x0[3]=x[3];
  float maxV = 0.0;
  for(int iu=0;iu<nn1;iu++){
    fun_discrete(x,U1[iu]);
    V_u = four_d_interp(x);
    x[0]=x0[0]; x[1]=x0[1]; x[2]=x0[2]; x[3]=x0[3];
    if(V_u>=maxV){
      maxV=V_u;
    }
  }
  //  printf("maxV = %f\n",maxV);
  float fac = maxV+yield-V[i][j][o][k];
  V[i][j][o][k] = V[i][j][o][k] + K[i][j][o][k]*fac;
  en[i][j][o][k] = fac;
}


void parallel(int i){
  float x[4];
  for(int j=0;j<n2;j++){
    for(int o=0;o<n3;o++){
      for(int k=0;k<n4;k++){
	  if(FLG[i][j][o][k]==1){
	    x[0]=x1_grid[i];
	    x[1]=x2_grid[j];
	    x[2]=x3_grid[o];
	    x[3]=x4_grid[k];
	    max_routine(x,i,j,o,k);
	  }
      }
    }
  }
}


float max_en(){
  float max_e = -10.0;
    for(int i=0;i<n1;i++){
      for(int j=0;j<n2;j++){
	for(int o=0;o<n3;o++){
	  for(int k=0;k<n4;k++){
	      if(fabs(en[i][j][o][k])>max_e)
		max_e = fabs(en[i][j][o][k]);
	    }
	  }
      }
    }
  return max_e;
}


int value_iteration(){
  FILE *fp;fp = fopen("./status.txt","w");fclose(fp);
  int count = 1;
  float max_e = 10.0;
  while(max_e>0.4){
#pragma omp parallel num_threads(NTHREADS)
#pragma omp for
    for(int i=0;i<n1;i++)
      parallel(i);
    if(count%50==0){
      fp = fopen("./status.txt","a"); fprintf(fp,"%d: %f\n",count,max_e);fclose(fp);
    }
    max_e = max_en();
    count++;
    printf("%d: max_err = %f\n",count,max_e);
  }
  return count;
}



void compute_control(float *x, float *u1){
  float V_u=0.0;
  float x0[4];
  float maxV=0.0;
  *u1=0.0;
  x0[0] = x[0]; x0[1] = x[1]; x0[2] = x[2]; x0[3] = x[3];
  for(int iu=0; iu<nn1; iu++){
    fun_discrete(x,U1[iu]);
    V_u = four_d_interp(x); 
    x[0] = x0[0]; x[1] = x0[1]; x[2] = x0[2]; x[3] = x0[3];
    if(V_u>maxV){
      maxV = V_u;
      *u1=U1[iu];
    }
  }
}


void simulate(float T, float x1_0, float x2_0, float x3_0, float x4_0){
  float time=0.0;
  int L = (int)(T/(float)dT + 1.0);
  float x[4];
  float U[L-1];
  float u1=0.0;
  float V_record[L-1];
  float X[L][4];
  X[0][0] = x1_0;
  X[0][1] = x2_0;
  X[0][2] = x3_0;
  X[0][3] = x4_0;
  for(int i=1;i<L;i++){
    x[0]=X[i-1][0]; x[1]=X[i-1][1]; x[2]=X[i-1][2]; x[3]=X[i-1][3];
    V_record[i-1] = four_d_interp(x); 
    compute_control(x,&u1);
    U[i-1]= u1; 
    fun_discrete(x,u1);
    X[i][0]=x[0]; X[i][1]=x[1]; X[i][2]=x[2]; X[i][3]=x[3]; 
    time = time+dT;
    if(check_notin_G(x)){
      L=i+1;
      printf("violated constraints at t = %f sec\n",i*dT);
      break;
    }
  }
  FILE *fp;
  char filename[75] = "/scratch/ilya_flux/robzidek/Inverted_Pendulum/sim_output_dT_0_01.txt";
  fp = fopen(filename,"w");
  fprintf(fp,"%f\n",dT);
  fprintf(fp,"%i\n",L);
  fprintf(fp,"%f\n",x1_grid[0]); fprintf(fp,"%f\n",x1_grid[n1-1]);
  fprintf(fp,"%f\n",x2_grid[0]); fprintf(fp,"%f\n",x2_grid[n2-1]);
  fprintf(fp,"%f\n",x3_grid[0]); fprintf(fp,"%f\n",x3_grid[n3-1]);
  fprintf(fp,"%f\n",x4_grid[0]); fprintf(fp,"%f\n",x4_grid[n4-1]);
  for(int i=0;i<L;i++){
    for(int j=0;j<4;j++)
      fprintf(fp,"%.14f\n",X[i][j]);
  }
  for(int i=0;i<L-1;i++){
    fprintf(fp,"%f\n",U[i]);
  }
  for(int i=0;i<L-1;i++)
    fprintf(fp,"%f\n",V_record[i]);
  fclose(fp);
}

void write2file(){
  FILE *fp;
  char filename[75] = "/scratch/ilya_flux/robzidek/Inverted_Pendulum/V_dT_0_01.txt";
  fp = fopen(filename,"w");
  fprintf(fp,"%f\n",(double)n1);
  fprintf(fp,"%f\n",(double)n2);
  fprintf(fp,"%f\n",(double)n3);
  fprintf(fp,"%f\n",(double)n4);
  fprintf(fp,"%f\n",(double)nn1);
  for(int i=0;i<n1;i++)
    fprintf(fp,"%f\n",x1_grid[i]);
  for(int i=0;i<n2;i++)
    fprintf(fp,"%f\n",x2_grid[i]);
  for(int i=0;i<n3;i++)
    fprintf(fp,"%f\n",x3_grid[i]);
  for(int i=0;i<n4;i++)
    fprintf(fp,"%f\n",x4_grid[i]);
  for(int i=0;i<nn1;i++)
    fprintf(fp,"%f\n",U1[i]);
  for(int i=0;i<n1;i++){
    for(int j=0;j<n2;j++){
      for(int o=0;o<n3;o++){
	for(int k=0;k<n4;k++){
	    fprintf(fp,"%f\n",V[i][j][o][k]);
	  }
	}
      }
  }
  /*
  for(int i=0;i<iter_count;i++)
    fprintf(fp,"%f\n",en_iter[i]);
  */
  fclose(fp);
}
