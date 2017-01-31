#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"


int find_nearest_neighbor(float x, float *x_grid, float x_step,  int n, float shift){
  int ix=0;
  if(x<x_grid[0] || x>x_grid[n-1]){
    ix = -1;
    return ix;
  }
  ix = (int)round((x+shift)/x_step);
  if((x-x_grid[ix])<0 || ix==n-1)
    ix--;
  return ix;
}



int find_neighbors(float x, float *x_grid, int n){
  int ix=0;
  if(x==x_grid[0]){
    ix=0;
  }
  else if (x==x_grid[n-1]){
    ix=n-2;
  }
  else if (x<x_grid[0] || x>x_grid[n-1]){
    ix=-1;
  }
  else{
    for(int i=0;i<(n-1);i++){
      if (x>=x_grid[i] && x<x_grid[i+1]){
	ix=i;
	break;
      }
    }
  }
  return ix;
}


float inter(float t, float a0, float a1){
  float out = t*a1+(1-t)*a0;
  return out;
}


float four_d_interp(float *x){
  float out;
  int ix1[2];
  int ix2[2];
  int ix3[2];
  int ix4[2];
  int ix=0;
  ix=find_neighbors(x[0],x1_grid,n1);ix1[0]=ix;ix1[1]=ix+1;
  ix=find_neighbors(x[1],x2_grid,n2);ix2[0]=ix;ix2[1]=ix+1;
  ix=find_neighbors(x[2],x3_grid,n3);ix3[0]=ix;ix3[1]=ix+1;
  ix=find_neighbors(x[3],x4_grid,n4);ix4[0]=ix;ix4[1]=ix+1;
  if(ix1[0]==-1 || ix2[0]==-1 || ix3[0]==-1 || ix4[0]==-1){
    out = 0; // dont extrapolate, set out to 0
    return out;
  }
  float x1step = x1_grid[ix1[1]]-x1_grid[ix1[0]];
  float x2step = x2_grid[ix2[1]]-x2_grid[ix2[0]];
  float x3step = x3_grid[ix3[1]]-x3_grid[ix3[0]];
  float x4step = x4_grid[ix4[1]]-x4_grid[ix4[0]];
  float t0 = (x[0]-x1_grid[ix1[0]])/x1step;
  float t1 = (x[1]-x2_grid[ix2[0]])/x2step;
  float t2 = (x[2]-x3_grid[ix3[0]])/x3step;
  float t3 = (x[3]-x4_grid[ix4[0]])/x4step;
  float V_inter[14];
  V_inter[0] = inter(t0,V[ix1[0]][ix2[0]][ix3[0]][ix4[0]],V[ix1[1]][ix2[0]][ix3[0]][ix4[0]]);
  V_inter[1] = inter(t0,V[ix1[0]][ix2[1]][ix3[0]][ix4[0]],V[ix1[1]][ix2[1]][ix3[0]][ix4[0]]);
  V_inter[2] = inter(t0,V[ix1[0]][ix2[0]][ix3[1]][ix4[0]],V[ix1[1]][ix2[0]][ix3[1]][ix4[0]]);
  V_inter[3] = inter(t0,V[ix1[0]][ix2[1]][ix3[1]][ix4[0]],V[ix1[1]][ix2[1]][ix3[1]][ix4[0]]);
  V_inter[4] = inter(t0,V[ix1[0]][ix2[0]][ix3[0]][ix4[1]],V[ix1[1]][ix2[0]][ix3[0]][ix4[1]]);
  V_inter[5] = inter(t0,V[ix1[0]][ix2[1]][ix3[0]][ix4[1]],V[ix1[1]][ix2[1]][ix3[0]][ix4[1]]);
  V_inter[6] = inter(t0,V[ix1[0]][ix2[0]][ix3[1]][ix4[1]],V[ix1[1]][ix2[0]][ix3[1]][ix4[1]]);
  V_inter[7] = inter(t0,V[ix1[0]][ix2[1]][ix3[1]][ix4[1]],V[ix1[1]][ix2[1]][ix3[1]][ix4[1]]);
  V_inter[8] = inter(t1,V_inter[0],V_inter[1]);
  V_inter[9] = inter(t1,V_inter[2],V_inter[3]);
  V_inter[10] = inter(t1,V_inter[4],V_inter[5]);
  V_inter[11] = inter(t1,V_inter[6],V_inter[7]);
  V_inter[12] = inter(t2,V_inter[8],V_inter[9]);
  V_inter[13] = inter(t2,V_inter[10],V_inter[11]);
  out = inter(t3,V_inter[12],V_inter[13]);
  //  printf("out = %f\n",out);
  return out;
}



