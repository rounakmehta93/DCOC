//
//  interpolation.c
//  DCOC_in_C
//
//  Created by Rounak on 7/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//
#include "globals.h"
#include "interpolation.h"
#include "math.h"

extern int x1_len, x2_len, w_len, u_len;

int find_nearest_neighbor(float val, float step_size, float min, float max){
    int ix;
    if(val < min || val > max)
        return -1;
    ix = ((val - min)/step_size);
    if(val == max){
        ix--;
    }
    return(ix);
}

float interpol_1D(float v0, float v1, float x0, float val, float step_size){
    
    return (v0 + (v1 - v0) * (val - x0)/step_size );
}

/*
float interpol_2D(float **v, float *x1, float *x2, float val, float step_size[2], int x1_len, int x2_len){
    int ix1, ix2;
    ix1 = find_nearest_neighbor(val, step_size[0], x1[0], x1[x1_len-1]);
    ix2 = find_nearest_neighbor(val, step_size[1], x2[0], x2[x2_len-1]);
    
}
 */

float interpol_3D(float x1_space[x1_len], float x2_space[x2_len], float w_space[w_len], float V[x1_len][x2_len][w_len], float x1,float  x2, float w){
    //method for trilinear inerpolation. implement recursive way later. also search should be binary.
    //find the grid to interpolate in
    
    //returns 0 when out of bounds
    float V_inter[2][2];
    float V_inter2[2];
    float V_inter3;
    float step_size[3];
    step_size[0] = x1_space[1]-x1_space[0];
    step_size[1] = x2_space[1]-x2_space[0];
    step_size[2] = w_space[1]-w_space[0];
    
    int ix1,ix2,ix3;
    ix1 = find_nearest_neighbor(x1 ,step_size[0], x1_space[0], x1_space[x1_len-1]);
    ix2 = find_nearest_neighbor(x2 ,step_size[1], x2_space[0], x2_space[x2_len-1]);
    ix3 = find_nearest_neighbor(w ,step_size[2], w_space[0], w_space[w_len-1]);
    
    printf("\n ix1 %d, ix2 %d ,ix3 %d\n", ix1, ix2, ix3);
    if(ix1==-1 || ix2==-1 || ix3==-1){
        return 0;
    }
    
    V_inter[0][0] = interpol_1D(V[ix1][ix2][ix3], V[ix1][ix2][ix3+1], w_space[ix3], w, step_size[2]);
    V_inter[1][0] = interpol_1D(V[ix1+1][ix2][ix3], V[ix1+1][ix2][ix3+1], w_space[ix3], w, step_size[2]);
    V_inter[0][1] = interpol_1D(V[ix1][ix2+1][ix3], V[ix1][ix2+1][ix3+1], w_space[ix3], w, step_size[2]);
    V_inter[1][1] = interpol_1D(V[ix1+1][ix2+1][ix3], V[ix1+1][ix2+1][ix3+1], w_space[ix3], w, step_size[2]);
    printf("%f",V[0][2][ix3]);
    printf("\nV_inter %f %f %f %f\n",V_inter[0][0], V_inter[1][0], V_inter[0][1], V_inter[1][1]);
    
    V_inter2[0] = interpol_1D(V_inter[0][0], V_inter[0][1], x2_space[ix2], x2, step_size[1]);
    V_inter2[1] = interpol_1D(V_inter[1][0], V_inter[1][1], x2_space[ix2], x2, step_size[1]);
    
    V_inter3 = interpol_1D(V_inter2[0], V_inter2[1], x1_space[ix1], x1, step_size[0]);
    
    
    return V_inter3;
    
    
    
}

