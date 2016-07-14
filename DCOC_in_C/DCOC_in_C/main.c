//
//  main.c
//  DCOC_in_C
//
//  Created by Rounak on 29/6/16.
//  Copyright © 2016 Rounak. All rights reserved.
// Doubts: Cannot declare array with length defined by a variable as global

#include "globals.h"
#include "globaltemp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //required for memcpy
#include <math.h>
#include "interpolation.h"
#include "pendulum_model.h"

#include "test_pendulum.h"
#define PI 3.14


extern int x1_len, x2_len, w_len, u_len;
extern float prob_matrix[3][3]; // need to specify lengths for multi dim arrays
extern float del_t;
extern float u_space[];
extern float w_space[];
extern float x_min[];
extern float x_max[];
extern float step_size[];
extern float stopping_criterion;
extern int num_states;


void pendulum_nonlinearmodel_ss(float X[], float U, float F, float x_new[]);
float interpol(float x1_space[], float x2_space[], float w_space[], float ***V, float x1,float x2,float w, int x1_len, int x2_len, int w_len);
float max_of_V(float V[x1_len][x2_len][w_len], float V_old[x1_len][x2_len][w_len]);
void deepcopy(float V[x1_len][x2_len][w_len], float V_old[x1_len][x2_len][w_len]);
int writeToFile(float V[x1_len][x2_len][w_len]);

int value_iteration(){
    //float lol = prob_matrix1[0][0];
    //printf("%f",lol);
    //init stuff
    static 
    int i,j,k;
    
    
    float V[x1_len][x2_len][w_len];
    float V_old[x1_len][x2_len][w_len];
    
    float x_new[] = {0,0};
    float x_old[] = {1,0};
    float g = 1;
    
    float x1_space[x1_len];
    float x2_space[x2_len];
    
    float temp_w[w_len];
    float temp_u[u_len];
    
    int c = 0, d=0;
    
    int max_temp_c = 0;
    int iteration = 0;
    
    
    for(i=0;i<x1_len;i++){
        x1_space[i] = x_min[0] + step_size[0]*i;
        //printf("%f ",x1_space[i]);
    }
     
    
    for(i=0;i<x2_len;i++){
        x2_space[i] = x_min[1] + step_size[1]*i;
    }
    
    
   
    for(i=0;i<x1_len;i++){
        for(j=0;j<x2_len;j++){
            for(k=0;k<w_len;k++){
                V[i][j][k] = 1.5;
                V_old[i][j][k] = 1;
                
            }
        }
    }
    
    //DEBUG AREA
    //printf("%d x1_len \n",x1_len);
    //printf("%f probmatrix\n",prob_matrix[0][0]);
    //printf("%f sc\n",stopping_criterion);
    //printf("%f x\n",x_new[1]);
    pendulum_nonlinearmodel_ss(x_old, u_space[c], w_space[k],x_new);
    //printf("%f x\n",x_new[1]);
    
    float debug;
    int deb;
    deb = find_nearest_neighbor(-0.1, 0.1, 0, 1);
    //printf("%d deb\n",deb);
    debug = interpol_3D(x1_space, x2_space, w_space, V, -0.2456,-0.8015, 0.1);//interpol might be off or V update
    
    printf("%f debug\n",debug);
   
    
    debug = max_of_V(V, V_old);
    
    
    
    
    while(max_of_V(V, V_old) > stopping_criterion){
        deepcopy(V, V_old);
        
        //printf("cpy %f %f \n",V[1][1][1],V_old[1][1][1]);
        for(i=0;i<x1_len;i++){
            for(j=0;j<x2_len;j++){
                for(k=0;k<w_len;k++){
                    max_temp_c = 0;
                    for(c=0;c<u_len;c++){
                        x_old[0] = x1_space[i];
                        x_old[1] = x2_space[j];
                        //printf("1 %f 2 %f 3 %f 4 %f\n", x1_space[i], x2_space[j],u_space[c], w_space[k]);
                        pendulum_nonlinearmodel_ss(x_old, u_space[c], w_space[k],x_new);
                        //printf("non linear %f %f \n",x_new[0],x_new[1]);
                        temp_u[c] = g;
                        for(d=0;d<w_len;d++){
                            temp_w[d] = interpol_3D(x1_space, x2_space, w_space, V, x_new[0], x_new[1], w_space[d]);
                            //printf("%f temp_w[d]\n",temp_w[d]);
                            temp_u[c] = temp_u[c] + temp_w[d]*prob_matrix[k][d];
                            
                        }
                        //printf("temp u[c] %f\n",temp_u[c]);
                        if(temp_u[c]>temp_u[max_temp_c]){
                            max_temp_c = c;
                        }
                        
                    }
                    
                    V[i][j][k] = temp_u[max_temp_c];
                    //printf("V[i][j][k] %f \n",V[i][j][k]);
                    //printf("V_old[i][j][k] %f \n",V_old[i][j][k]);
                    //printf("================\n");
                }
            }
         
        }
     
        printf("iteration %d value %f \n",iteration, max_of_V(V, V_old));
        
        iteration += 1;

    }
    
    
    


    
    
    printf("Value iteration Done\n Writing to file...\n");
    int status;
    status = writeToFile(V);
    printf("Done\n");
    
    return(0);
}

    int writeToFile(float V[x1_len][x2_len][w_len]){
        int i,j,k;
    FILE *outfile;
    char *outfilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_in_C/V_out.txt";
    printf("%s",outfilename);
    if ((outfile = fopen(outfilename,"w")) == NULL)
    {
        printf("Error opening output file.\n");
        return(-1);
    }
    else{
        printf("\nfile opened\n");
    }
    for(i=0;i<x1_len;i++)
    {
        for(j=0;j<x2_len;j++)
        {
            for(k=0;k<w_len;k++){
                fprintf(outfile,"%f\n",V[i][j][k]);
            }
            //fprintf(outfile,"::::::");
        }
        //fprintf(outfile,"\n");
    }
    fclose(outfile);
    
    return 0;
}

void pendulum_nonlinearmodel_ss(float X[num_states], float U, float F, float x_new[num_states]){
    
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
    //printf("%f x in function \n",x_new[1]);
    
    
}
float max_of_V(float V[x1_len][x2_len][w_len], float V_old[x1_len][x2_len][w_len]){
    int i, j, k;
    float temp;
    float max = fabs(V[0][0][0]-V_old[0][0][0]);
    for(i=0;i<x1_len;i++){
        for(j=0;j<x2_len;j++){
            for(k=0;k<w_len;k++){
                temp = fabs(V[i][j][k]-V_old[i][j][k]);
                if(temp > max){
                    max = temp;
                }
            }
        }
    }
    return max;
}

void deepcopy(float V[x1_len][x2_len][w_len], float V_old[x1_len][x2_len][w_len]){
    int i, j, k;
    for(i=0;i<x1_len;i++){
        for(j=0;j<x2_len;j++){
            for(k=0;k<w_len;k++){
                V_old[i][j][k] = V[i][j][k];
                
            }
        }
    }
}

void test_interpol(){
    float x1_space[3] = {1,2,3};
    float x2_space[3] = {1,2,3};
    float w_space[3] = {1,2,3};
    x1_len = 3;
    x2_len = 3;
    w_len = 3;
    float V[3][3][3] = {{{10,10,10},{20,20,20},{30,30,30}},{{40,40,40},{50,50,50},{60,60,60}},{{70,70,70},{80,80,80},{90,90,90}}};
    float x_new[2] = {3,3.1};
    int d = 1;
    float ans;
    printf("\nbig lol%f",V[1][1][1]);
    ans = interpol_3D(x1_space, x2_space, w_space, V, x_new[0], x_new[1], w_space[d]);
    printf("ans %f \n ",ans);
}

void test_max(){
    x1_len = 3;
    x2_len = 3;
    w_len = 3;
    float V[3][3][3] = {{{10,10,10},{20,20,20},{30,30,30}},{{40,40,40},{50,50,50},{60,60,60}},{{70,70,70},{80,80,80},{90,90,90}}};
    float V_old[3][3][3] = {{{30,10000,10},{20,20,20},{30,30,30}},{{40,40,40},{50,50,50},{60,60,60}},{{70,70,70},{80,80,80},{90,90,90}}};
    float ans;
    
    ans = max_of_V(V, V_old);
    printf("ans %f \n ",ans);
}

void test_copy(){
    x1_len = 3;
    x2_len = 3;
    w_len = 3;
    float V[3][3][3] = {{{10,10,10},{20,20,20},{30,30,30}},{{40,40,40},{50,50,50},{60,60,60}},{{70,70,70},{80,80,80},{90,90,90}}};
    float V_old[3][3][3] = {{{20,10,10},{20,20,20},{30,30,30}},{{40,40,40},{50,50,50},{60,60,60}},{{70,70,70},{80,80,80},{90,90,90}}};
    float ans;
    
    deepcopy(V, V_old);
    V[0][0][0] = 10000;
    printf("ans %f \n ",V_old[0][0][0]);
}

void test_load(){
    x1_len = 3;
    x2_len = 3;
    w_len = 3;
    float V[3][3][3] = {{{10,10,10},{20,32030,20},{30,30,32230}},{{40,4090,40},{50,50,5087},{60,620,60}},{{710,700,720},{8110,810,80},{90,90,901}}};
    float V_loaded[3][3][3];
    int status;
    status = writeToFile(V);
    x1_len = 3;
    x2_len = 3;
    w_len = 3;
    load_V(V_loaded);
    printf("V %f V_loaded %f",V[2][2][2],V_loaded[2][2][2]);
    printf("V %f V_loaded %f",V[2][1][2],V_loaded[2][1][2]);
    printf("V %f V_loaded %f",V[1][1][0],V_loaded[1][1][0]);
    printf("V %f V_loaded %f",V[0][1][2],V_loaded[0][1][2]);
    
}



int main(int argc, const char * argv[]) {
    // Debug area
    arr[1] = 12;
    printf("Hello, World!\n %d\n",arr[1]);
    
    int status;
    
    init_value_iteration();
    //serious shit starts here:
    //test_interpol();
    //test_max();
    //test_copy();
    //test_load();
    //status = value_iteration();
    test_pendulum();
    return 0;
}


