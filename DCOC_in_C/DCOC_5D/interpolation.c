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
#include "test_pendulum.h" //for indexOfw()

extern double V[x1_len][x2_len][x2_len][x3_len][w_len];
extern double w_space[];
extern double x0_space[],x1_space[],x2_space[],x3_space[];
extern double step_size[num_states];

int find_nearest_neighbor(double val, double step_size, double min, double max){
    int ix;
    if(val < min || val > max)
        return -1;
    ix = ((val - min)/step_size);
    if(val == max){
        ix--;
    }
    return(ix);
}

double interpol_1D(double v0, double v1, double x0, double val, double step_size){
    
    return (v0 + (v1 - v0) * (val - x0)/step_size );
}



double interpol_5D(double x0, double x1,double  x2, double  x3, double w){
    //method for trilinear inerpolation. implement recursive way later. also search should be binary.
    //find the grid to interpolate in
    
    //returns 0 when out of bounds
    double V_inter[2][2][2][2];
    double V_inter1[2][2][2];
    double V_inter2[2][2];
    double V_inter3[2];
    double V_inter4;
    double step_size_w;
    
    step_size_w = w_space[1]-w_space[0];
    if(w_len == 1){
        printf("w len %d",w_len);
        step_size_w = 0.00001; //to avoid 0/0
    }
    
    int ix0,ix1,ix2,ix3,ix4;
    ix0 = find_nearest_neighbor(x0 ,step_size[0], x0_space[0], x0_space[x0_len-1]);
    ix1 = find_nearest_neighbor(x1 ,step_size[1], x1_space[0], x1_space[x1_len-1]);
    ix2 = find_nearest_neighbor(x2 ,step_size[2], x2_space[0], x2_space[x2_len-1]);
    ix3 = find_nearest_neighbor(x3 ,step_size[3], x3_space[0], x3_space[x3_len-1]);
    ix4 = find_nearest_neighbor(w ,step_size_w, w_space[0], w_space[w_len-1]);
    
    //printf("\n ix1 %d, ix2 %d ,ix3 %d\n", ix1, ix2, ix3);
    if(ix0==-1 | ix1==-1 || ix2==-1 || ix3==-1 || ix4==-1){
        return 0;
    }
    
    V_inter[0][0][0][0] = interpol_1D(V[ix0][ix1][ix2][ix3][ix4], V[ix0][ix1][ix2][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[0][0][0][1] = interpol_1D(V[ix0][ix1][ix2][ix3+1][ix4], V[ix0][ix1][ix2][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[0][0][1][0] = interpol_1D(V[ix0][ix1][ix2+1][ix3][ix4], V[ix0][ix1][ix2+1][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[0][0][1][1] = interpol_1D(V[ix0][ix1][ix2+1][ix3+1][ix4], V[ix0][ix1][ix2+1][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[0][1][0][0] = interpol_1D(V[ix0][ix1+1][ix2][ix3][ix4], V[ix0][ix1+1][ix2][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[0][1][0][1] = interpol_1D(V[ix0][ix1+1][ix2][ix3+1][ix4], V[ix0][ix1+1][ix2][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[0][1][1][0] = interpol_1D(V[ix0][ix1+1][ix2+1][ix3][ix4], V[ix0][ix1+1][ix2+1][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[0][1][1][1] = interpol_1D(V[ix0][ix1+1][ix2+1][ix3+1][ix4], V[ix0][ix1+1][ix2+1][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    
    V_inter[1][0][0][0] = interpol_1D(V[ix0+1][ix1][ix2][ix3][ix4], V[ix0+1][ix1][ix2][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[1][0][0][1] = interpol_1D(V[ix0+1][ix1][ix2][ix3+1][ix4], V[ix0+1][ix1][ix2][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[1][0][1][0] = interpol_1D(V[ix0+1][ix1][ix2+1][ix3][ix4], V[ix0+1][ix1][ix2+1][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[1][0][1][1] = interpol_1D(V[ix0+1][ix1][ix2+1][ix3+1][ix4], V[ix0+1][ix1][ix2+1][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[1][1][0][0] = interpol_1D(V[ix0+1][ix1+1][ix2][ix3][ix4], V[ix0+1][ix1+1][ix2][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[1][1][0][1] = interpol_1D(V[ix0+1][ix1+1][ix2][ix3+1][ix4], V[ix0+1][ix1+1][ix2][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    V_inter[1][1][1][0] = interpol_1D(V[ix0+1][ix1+1][ix2+1][ix3][ix4], V[ix0+1][ix1+1][ix2+1][ix3][ix4+1], w_space[ix4], w, step_size_w);
    V_inter[1][1][1][1] = interpol_1D(V[ix0+1][ix1+1][ix2+1][ix3+1][ix4], V[ix0+1][ix1+1][ix2+1][ix3+1][ix4+1], w_space[ix4], w, step_size_w);
    
    
    //printf("\nV_inter %f %f %f %f\n",V_inter[0][0], V_inter[1][0], V_inter[0][1], V_inter[1][1]);
    
    V_inter1[0][0][0] = interpol_1D(V_inter[0][0][0][0], V_inter[0][0][0][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][0][1] = interpol_1D(V_inter[0][0][1][0], V_inter[0][0][1][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][1][0] = interpol_1D(V_inter[0][1][0][0], V_inter[0][1][0][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][1][1] = interpol_1D(V_inter[0][1][1][0], V_inter[0][1][1][1],x3_space[ix3], x3, step_size[3]);
    
    V_inter1[1][0][0] = interpol_1D(V_inter[1][0][0][0], V_inter[1][0][0][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][0][1] = interpol_1D(V_inter[1][0][1][0], V_inter[1][0][1][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][1][0] = interpol_1D(V_inter[1][1][0][0], V_inter[1][1][0][1],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][1][1] = interpol_1D(V_inter[1][1][1][0], V_inter[1][1][1][1],x3_space[ix3], x3, step_size[3]);
    
    
    V_inter2[0][0] = interpol_1D(V_inter1[0][0][0], V_inter1[0][0][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[0][1] = interpol_1D(V_inter1[0][1][0], V_inter1[0][1][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[1][0] = interpol_1D(V_inter1[1][0][0], V_inter1[1][0][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[1][1] = interpol_1D(V_inter1[1][1][0], V_inter1[1][1][1],x2_space[ix2], x2, step_size[2]);
    
    V_inter3[0] = interpol_1D(V_inter2[0][0], V_inter2[0][1],x1_space[ix1], x1, step_size[1]);
    V_inter3[1] = interpol_1D(V_inter2[1][0], V_inter2[1][1],x1_space[ix1], x1, step_size[1]);
    
    //printf("V_inter2 %f %f \n", V_inter2[0], V_inter2[1]);
    V_inter4 = interpol_1D(V_inter3[0], V_inter3[1], x0_space[ix0], x0, step_size[0]);
    
    
    return V_inter4;
    
    
    
}


double interpol_4D(double x0, double x1,double  x2, double  x3, double w){
    
    
    //returns 0 when out of bounds
    
    double V_inter1[2][2][2];
    double V_inter2[2][2];
    double V_inter3[2];
    double V_inter4;
    
    
    
    
    int ix0,ix1,ix2,ix3,iw;
    ix0 = find_nearest_neighbor(x0 ,step_size[0], x0_space[0], x0_space[x0_len-1]);
    ix1 = find_nearest_neighbor(x1 ,step_size[1], x1_space[0], x1_space[x1_len-1]);
    ix2 = find_nearest_neighbor(x2 ,step_size[2], x2_space[0], x2_space[x2_len-1]);
    ix3 = find_nearest_neighbor(x3 ,step_size[3], x3_space[0], x3_space[x3_len-1]);
    iw = indexOfw(w);
    
    //printf("\n ix1 %d, ix2 %d ,ix3 %d\n", ix1, ix2, ix3);
    if(ix0==-1 | ix1==-1 || ix2==-1 || ix3==-1){
        return 0;
    }
    
    
    
    
    //printf("\nV_inter %f %f %f %f\n",V_inter[0][0], V_inter[1][0], V_inter[0][1], V_inter[1][1]);
    
    V_inter1[0][0][0] = interpol_1D(V[0][0][0][0][iw], V[0][0][0][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][0][1] = interpol_1D(V[0][0][1][0][iw], V[0][0][1][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][1][0] = interpol_1D(V[0][1][0][0][iw], V[0][1][0][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[0][1][1] = interpol_1D(V[0][1][1][0][iw], V[0][1][1][1][iw],x3_space[ix3], x3, step_size[3]);
    
    V_inter1[1][0][0] = interpol_1D(V[1][0][0][0][iw], V[1][0][0][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][0][1] = interpol_1D(V[1][0][1][0][iw], V[1][0][1][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][1][0] = interpol_1D(V[1][1][0][0][iw], V[1][1][0][1][iw],x3_space[ix3], x3, step_size[3]);
    V_inter1[1][1][1] = interpol_1D(V[1][1][1][0][iw], V[1][1][1][1][iw],x3_space[ix3], x3, step_size[3]);
    
    V_inter2[0][0] = interpol_1D(V_inter1[0][0][0], V_inter1[0][0][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[0][1] = interpol_1D(V_inter1[0][1][0], V_inter1[0][1][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[1][0] = interpol_1D(V_inter1[1][0][0], V_inter1[1][0][1],x2_space[ix2], x2, step_size[2]);
    V_inter2[1][1] = interpol_1D(V_inter1[1][1][0], V_inter1[1][1][1],x2_space[ix2], x2, step_size[2]);
    
    V_inter3[0] = interpol_1D(V_inter2[0][0], V_inter2[0][1],x1_space[ix1], x1, step_size[1]);
    V_inter3[1] = interpol_1D(V_inter2[1][0], V_inter2[1][1],x1_space[ix1], x1, step_size[1]);
    
    //printf("V_inter2 %f %f \n", V_inter2[0], V_inter2[1]);
    V_inter4 = interpol_1D(V_inter3[0], V_inter3[1], x0_space[ix0], x0, step_size[0]);
    
    
    return V_inter4;
    
    
    
}

