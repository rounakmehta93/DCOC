//
//  main.c
//  DCOC_in_C
//
//  Created by Rounak on 29/6/16.
//  Copyright © 2016 Rounak. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //required for memcpy
#include <math.h>
#define PI = 3.14159 //for some reason can't use this in the code

//INIT

//need to specify length in 2nd argument
const float pi = 3.14159;
const float prob_matrix[][3] = {{0.5, 0.9, 0.4}, {0.1, 0.2, 0.7}, {0.3, 0.3, 0.4}};
const float del_t = 0.01;
const float u_space[] = {-0.01,0,0.01};
float w_space[] = {-0.1,0,0.1};
const float x_min[] = {-pi/8,-1};
const float x_max[] = {pi/8,1};
const float step_size[] = {pi/20,0.1};


void pendulum_nonlinearmodel_ss(float X[], float U, float F, float del_t, float x_new[]);
float interpol(float x1_space[], float x2_space[], float w_space[], float ***V, float x1,float x2,float w, int x1_len, int x2_len, int w_len);
float max_of_V(float ***V, float ***V_old, int x1_len, int x2_len, int w_len);
void deepcopy(float ***V, float ***V_old, int x1_len, int x2_len,int w_len);

void value_iteration(){
    
    //init stuff
    float stopping_criterion = 0.01;
    float *x1_space, *x2_space, ***V, ***V_old, *temp_w, *temp_u;
    int i,j,k;
    int x1_len = (x_max[0]-x_min[0])/step_size[0] + 1;
    int x2_len = (x_max[1]-x_min[1])/step_size[1] + 1;
    int w_len = sizeof(w_space)/sizeof(float);
    int u_len = sizeof(u_space)/sizeof(float);
    float x_new[] = {0,0};
    float g = 1;
    
    x1_space = (float *)(malloc(x1_len*sizeof(float)));
    for(i=0;i<x1_len;i++){
        x1_space[i] = x_min[0] + step_size[0]*i;
        //printf("%f ",x1_space[i]);
    }
    printf("\n");
    
    x2_space = (float *)(malloc(x2_len*sizeof(float)));
    for(i=0;i<x2_len;i++){
        x2_space[i] = x_min[1] + step_size[1]*i;
        //printf("%f ",x2_space[i]);
    }
    
    temp_w = (float *)(malloc(w_len*sizeof(float)));
    temp_u = (float *)(malloc(u_len*sizeof(float)));
    
    V = (float ***)(malloc(x1_len*sizeof(float *)));
    V_old = (float ***)(malloc(x1_len*sizeof(float *)));
    for(i=0;i<x1_len;i++){
        V[i] = (float **)(malloc(x2_len*sizeof(float *)));
        V_old[i] = (float **)(malloc(x2_len*sizeof(float *)));
        for(j=0;j<x2_len;j++){
            V[i][j]=(float *)(malloc(w_len*sizeof(float)));
            V_old[i][j]=(float *)(malloc(w_len*sizeof(float)));
            for(k=0;k<w_len;k++){
                V[i][j][k] = 0;
                V_old[i][j][k] = 1;
                
            }
        }
    }
    
    
    float a[] = {0,0};
    float x1=0.1,x2=0.1,w=0.1;
    pendulum_nonlinearmodel_ss(&a[0], 1, 0, del_t, &x_new[0]);
    printf("array gotten %f,%f\n",x_new[0],x_new[1]);
    
    float temp;
    temp= interpol(x1_space, x2_space, w_space, V, x1, x2, w,  x1_len, x2_len, w_len);
    printf("d %f",temp);
    //V_old = memcpy(&V[0][0][0], &V_old[0][0][0], x1_len*x2_len*w_len*sizeof(float));
    
    i = 0, j=0, k=0;
    int c = 0, d=0;
    float x_old[] = {0,0};
    float max_temp_u = 0;
    int iteration = 0;
    while(max_of_V(V, V_old, x1_len, x2_len, w_len) > stopping_criterion){
        deepcopy(V, V_old, x1_len, x2_len, w_len);
        //printf("cpy %f %f \n",V[1][1][1],V_old[1][1][1]);
        for(i=0;i<x1_len;i++){
            for(j=0;j<x2_len;j++){
                for(k=0;k<w_len;k++){
                    for(c=0;c<u_len;c++){
                        x_old[0] = x1_space[i];
                        x_old[1] = x2_space[j];
                        pendulum_nonlinearmodel_ss(&x_old[0], u_space[c], w_space[k], del_t, &x_new[0]);
                        //printf("non linear %f %f \n",x_new[0],x_new[1]);
                        temp_u[c] = g;
                        for(d=0;d<w_len;d++){
                            temp_w[d] = interpol(x1_space, x2_space, w_space, V, x_new[0], x_new[1], w_space[d],  x1_len, x2_len, w_len);
                            
                            temp_u[c] = temp_u[c] + temp_w[d]*prob_matrix[k][d];
                            //printf("temp w[d] %f\n",temp_w[d]);
                        }
                        
                        if(temp_u[c]>max_temp_u){
                            max_temp_u = temp_u[c];
                        }
                        
                    }
                    V[i][j][k] = max_temp_u;
                }
            }
        }
        printf("iteration %d value %f \n",iteration, max_of_V(V, V_old, x1_len, x2_len, w_len));
        
        iteration += 1;

    }
    
    
    


    
    
    printf("Value iteration Done\n");
    
}

void pendulum_nonlinearmodel_ss(float X[], float U, float F, float del_t, float x_new[]){
    
    float m = 0.2;
    float b = 0.001;
    float I = 0.006;
    float g = 9.8;
    float l = 0.3;
    float th = X[0];
    float th_ = X[1];
    float T = U;
    x_new[0] = X[0] + del_t* th_;
    x_new[1] = X[1] + del_t* (-m*g*l*sin(th) -b*th_ + F*l*cos(th) + T)/I; //cos and sin in radians
    
    
}

float interpol(float x1_space[], float x2_space[], float w_space[], float ***V, float x1,float  x2, float w, int x1_len, int x2_len, int w_len){
    //method for trilinear inerpolation. implement recursive way later. also search should be binary.
    //find the grid to interpolate in
    
    //returns 0 when out of bounds
    float a1, a2, b1, b2, c1, c2;
    float A1, A2, A3, A4, B1, B2, C;
    int a, b, c;
    short return_flag = -3;
    
    
    for(a=1; a < x1_len; a++){
        if(x1 >= x1_space[a-1] && x1 <=x1_space[a] ){
            a1 = x1_space[a-1];
            a2 = x1_space[a];
            return_flag += 1;
            break;
        }
    }
   
    for(b=1; b < x2_len; b++){
        if(x2 >= x2_space[b-1] && x2 <=x2_space[b] ){
            b1 = x2_space[b-1];
            b2 = x2_space[b];
            return_flag += 1;
            break;
        }
        
    }
    for(c=1; c < w_len; c++){
        if(w >= w_space[c-1] && w <=w_space[c] ){
            c1 = w_space[c-1];
            c2 = w_space[c];
            return_flag += 1;
            break;
        }
    }
    
    if(return_flag < 0){
        return(0);
    }
    

    
    
    
    A1 = V[a-1][b-1][c-1] + (V[a-1][b-1][c] - V[a-1][b-1][c-1])*(w - c1)/(c2-c1);
    A3 = V[a][b-1][c-1] + (V[a][b-1][c] - V[a][b-1][c-1])*(w - c1)/(c2-c1);
    A2 = V[a-1][b][c-1] + (V[a-1][b][c] - V[a-1][b][c-1])*(w - c1)/(c2-c1);
    A4 = V[a][b][c-1] + (V[a][b][c] - V[a][b][c-1])*(w - c1)/(c2-c1);
    
    
    B1 = A1 + (A2-A1)*(x2-b1)/(b2-b1);
    B2 = A3 + (A4-A3)*(x2-b1)/(b2-b1);
    
    C = B1 + (B2 - B1)*(x1-a1)/(a2-a1);
    
    return C;
    
}

float max_of_V(float ***V, float ***V_old, int x1_len, int x2_len, int w_len){
    int i, j, k;
    float temp;
    float max = fabs(V[0][0][0]-V_old[0][0][0]);
    for(i=0;i<x1_len;i++){
        for(j=0;j<x2_len;j++){
            for(k=0;k<w_len;k++){
                temp = fabs(V[0][0][0]-V_old[0][0][0]);
                if(temp > max){
                    max = temp;
                }
            }
        }
    }
    return max;
}

void deepcopy(float ***V, float ***V_old, int x1_len, int x2_len,int w_len){
    int i, j, k;
    for(i=0;i<x1_len;i++){
        for(j=0;j<x2_len;j++){
            for(k=0;k<w_len;k++){
                V_old[i][j][k] = V[i][j][k];
                
            }
        }
    }
}




int main(int argc, const char * argv[]) {
    // Debug area
    
    printf("Hello, World!\n %.2f\n",2.0);
    
    
    //serious shit starts here:
    value_iteration();
    return 0;
}
