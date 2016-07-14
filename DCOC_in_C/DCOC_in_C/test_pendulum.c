//
//  test_pendulum.c
//  DCOC_in_C
//
//  Created by Rounak on 8/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//

#include "test_pendulum.h"
#include "pendulum_model.h"
#include "interpolation.h"
#include <stdlib.h>
#include <time.h>


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
int arr_len;


void test_pendulum(){
    srand ( (uint)time(NULL) ); //otherwise we get same random number sequence.
    float u, w = w_space[0];
    float t_max = 20;
    float x[2] = {0,0};
    arr_len = t_max/del_t + 1;
    int i,exit_i = arr_len;
    float x_array[2][arr_len], t_array[arr_len], u_array[arr_len];
    
    float x1_space[x1_len];
    float x2_space[x2_len];
    for(i=0;i<x1_len;i++){
        x1_space[i] = x_min[0] + step_size[0]*i;
        //printf("%f ",x1_space[i]);
    }
    
    
    for(i=0;i<x2_len;i++){
        x2_space[i] = x_min[1] + step_size[1]*i;
    }
    
    for(i=0;i<arr_len;i++){
        t_array[i] = 0 + del_t*i;
    }
    
    i = 0;
    x_array[0][i] = x[0];
    x_array[1][i] = x[1];
    
    float V[x1_len][x2_len][w_len];
    load_V(V);
    printf("arr len %d\n",arr_len);
    for(i=1;i<arr_len;i++){
        //printf("i %d x_array[0] %f\n",i, x_array[0][i]);
        if(x[0]>x_max[0] || x[0]<x_min[0] || x[1]>x_max[1] || x[1]<x_min[1]){
            printf("exited state boundary at iteration %d\n", i);
            exit_i = i;
            break;
        }
        u = get_u_value_iteration(V,x,w,x1_space, x2_space);
        w = markov_model(w);
        //printf("w %f\n",w);
        pendulum_nonlinearmodel_ss1(x, u, w, x);
        x_array[0][i] = x[0];
        x_array[1][i] = x[1];
        u_array[i] = u;
        
    }
    writeToFileXU(x_array,u_array,t_array,exit_i);

}

float get_u_value_iteration(float V[x1_len][x2_len][w_len],float x[num_states],float w, float x1_space[x1_len], float x2_space[x2_len]){
    float g = 1;
    float x_new[num_states];
    float temp_w[w_len];
    float temp_u[u_len];
    int k;
    int max_temp_c=0;
    for(int c=0;c<u_len;c++){
        pendulum_nonlinearmodel_ss1(x, u_space[c], w, x_new);
        
        temp_u[c] = g;
        for(int d=0;d<w_len;d++){
            temp_w[d] = interpol_3D(x1_space, x2_space, w_space, V, x_new[0], x_new[1], w_space[d]);
            //printf("%f temp_w[d]\n",temp_w[d]);
            k = indexOfw(w);
            temp_u[c] = temp_u[c] + temp_w[d]*prob_matrix[k][d];
            
        }
        //printf("temp u[c] %f\n",temp_u[c]);
        if(temp_u[c]>temp_u[max_temp_c]){
            max_temp_c = c;
        }
        
    }
    return(u_space[max_temp_c]);
}

int indexOfw(float w){
    for(int i = 0; i < w_len; i++){
        //printf("w_space[i] %f\n",w_space[i]);
        //printf("w %f\n",w);
        if(w_space[i] == w){
            return(i);
        }
    }
    printf("w not found in w_space\n");
    exit(0);
}

void load_V(float V[x1_len][x2_len][w_len]){
    //printf("x1_len %d x2_len %d w_len %d", x1_len, x2_len, w_len);
    //printf("V %f, %f, %f\n",V[0][0][0], V[2][1][2], V[2][2][2]);
    int i=0,j=0,k=0;
    float number;
    FILE *infile;
    char *infilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_in_C/V_out.txt";
    printf("%s\n",infilename);
    if ((infile = fopen(infilename,"r")) == NULL)
    {
        printf("Error opening input file.\n");
        exit(0);
        }
    else{
        printf("\nfile opened\n");
    }
    
    while(fscanf(infile, "%f\n", &number) > 0 ){
        //printf("%f\n",number);
        V[i][j][k] = number;
        k++;
        if(k==w_len){
            k=0;
            j++;
            if(j==x2_len){
                j=0;
                i++;
            }
        }
        
    }
    //printf("x1_len %d i %d",x1_len,i);
    fclose(infile);

}

float markov_model(float w){
    int w_index = indexOfw(w);
    float row_sum;
    float prob_matrix_cdf[3][3];
    for (int i = 0; i< w_len; i++){
        row_sum=0;
        for(int j = 0; j< w_len; j++){
            row_sum = prob_matrix[i][j] + row_sum;
            //printf("%f",row_sum);
            prob_matrix_cdf[i][j] = row_sum;
            //printf("%f",prob_matrix[i][j]);
        }
    }
    
    float rndm = rand()/(float)RAND_MAX;
    //printf("rndm %f", rndm);
    for (int i = 0 ; i< w_len; i++){
        if(rndm <= prob_matrix_cdf[w_index][i]){
            //printf("w_space[i] %f\n",w_space[i]);
            return(w_space[i]);
        }
    }
    
    return(-1);
}


int writeToFileXU(float x_array[2][arr_len], float u_array[arr_len], float t_array[arr_len], int exit_i){
    int i;
    FILE *outfile;
    char *outfilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_in_C/XandU.txt";
    printf("%s",outfilename);
    if ((outfile = fopen(outfilename,"w")) == NULL)
    {
        printf("Error opening output file.\n");
        return(-1);
    }
    else{
        printf("\nfile opened. Output = t x[0] x[1] u\n");
    }
    fprintf(outfile,"%s %s %s %s\n","t","x1","x2","u");
    for(i=0;i<exit_i;i++)
    {
       fprintf(outfile,"%f %f %f %f\n",t_array[i],x_array[0][i], x_array[1][i],u_array[i]);
    }
    fclose(outfile);
    
    return 0;
}