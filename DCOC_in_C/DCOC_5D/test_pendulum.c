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
#include "globals.h"

extern double prob_matrix[PROB_MATRIX_LEN][PROB_MATRIX_LEN]; // need to specify lengths for multi dim arrays
extern double del_t;
extern double u_space[];
extern double w_space[];
extern double x0_space[],x1_space[],x2_space[],x3_space[];
extern double x_min[];
extern double x_max[];
extern double step_size[];
extern double stopping_criterion;

int arr_len;
extern double V[x1_len][x2_len][x2_len][x3_len][w_len];

void test_pendulum(){
    srand (time(NULL)); //otherwise we get same random number sequence.
    double u, w = w_space[0];
    int lol = 0;
    double t_max = 30;
    double x[num_states] = {0,0,0,0};
    arr_len = t_max/del_t + 1;
    int i,exit_i = arr_len;
    double x_array[num_states][arr_len], t_array[arr_len], u_array[arr_len];
    double w_array[arr_len];
    double V_array[arr_len];
    
    for(i=0;i<arr_len;i++){
        t_array[i] = 0 + del_t*i;
    }
    
    i = 0;
    x_array[0][i] = x[0];
    x_array[1][i] = x[1];
    x_array[2][i] = x[2];
    x_array[3][i] = x[3];
    
    load_V();
    printf("arr len %d\n",arr_len);
    for(i=1;i<arr_len;i++){
        //printf("i %d x_array[0] %lf\n",i, x_array[0][i]);
        if(x[0]>x_max[0] || x[0]<x_min[0] || x[1]>x_max[1] || x[1]<x_min[1] || x[2]>x_max[2] || x[2]<x_min[2] || x[3]>x_max[3] || x[3]<x_min[3]){
            printf("exited state boundary at iteration %d\n", i);
            exit_i = i;
            break;
        }
        u = get_u_value_iteration(x,w, &V_array[i]);
        w = markov_model(w);
        //printf("w %lf\n",w);
        pendulum_nonlinearmodel_ss(x, u, w, x, lol);
        x_array[0][i] = x[0];
        x_array[1][i] = x[1];
        x_array[2][i] = x[0];
        x_array[3][i] = x[1];
        u_array[i] = u;
        w_array[i] = w;
        
    }
    writeToFileXU(x_array,u_array,w_array,t_array,V_array,exit_i);

}

double get_u_value_iteration(double x[num_states],double w,double *V_in_arr){
    double g = 1;
    int lol = 1;
    double x_new[num_states];
    double temp_w[w_len];
    double temp_u[u_len];
    int k;
    int max_temp_c=0;
    double V_temp;
    for(int c=0;c<u_len;c++){
        pendulum_nonlinearmodel_ss(x, u_space[c], w, x_new, lol);
        
        temp_u[c] = g; //also reset for different u's
        V_temp = 0;
        for(int d=0;d<w_len;d++){
            temp_w[d] = interpol_4D(x_new[0], x_new[1], x_new[2],x_new[3], w_space[d]);
            //printf("%lf temp_w[d]\n",temp_w[d]);
            k = indexOfw(w);
            temp_u[c] = temp_u[c] + temp_w[d]*prob_matrix[k][d];
            V_temp = V_temp + temp_w[d]*prob_matrix[k][d]; //expected value of V_temp
            
        }
        //printf("temp u[c] %lf\n",temp_u[c]);
        if(c == 0){
            *V_in_arr = V_temp; //initialization
        }
        if(temp_u[c]>temp_u[max_temp_c]){
            max_temp_c = c;
            *V_in_arr = V_temp;
        }
        
    }
    return(u_space[max_temp_c]);
}

int indexOfw(double w){
    //printf("w %lf\n",w);
    for(int i = 0; i < w_len; i++){
        //printf("w_space[i] %lf\n",w_space[i]);
        //printf("w %lf\n",w);
        if(w_space[i] == w){
            return(i);
        }
    }
    printf("w not found in w_space\n");
    exit(0);
}

void load_V(){
    //printf("x1_len %d x2_len %d w_len %d", x1_len, x2_len, w_len);
    //printf("V %lf, %lf, %lf\n",V[0][0][0], V[2][1][2], V[2][2][2]);
    int i0=0,i1=0,i2=0,i3=0,i4=0;
    double number;
    FILE *infile;
    char *infilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_5D/V_out.txt";
    printf("%s\n",infilename);
    if ((infile = fopen(infilename,"r")) == NULL)
    {
        printf("Error opening input file.\n");
        exit(0);
        }
    else{
        printf("\nfile opened\n");
    }
    
    while(fscanf(infile, "%lf\n", &number) > 0 ){
        //printf("%lf\n",number);
        V[i0][i1][i2][i3][i4] = number;
        i4++;
        if(i4==w_len){
            i4=0;
            i3++;
            if(i3==x3_len){
                i3=0;
                i2++;
                if(i2==x2_len){
                    i2=0;
                    i1++;
                    if(i1==x1_len){
                        i1=0;
                        i0++;
                        if(i0==x0_len){
                            break;
                        }
                    }
                }
            }
        }
        
    }
    //printf("x1_len %d i %d",x1_len,i);
    fclose(infile);

}

double markov_model(double w){
    int w_index = indexOfw(w);
    double row_sum;
    double prob_matrix_cdf[PROB_MATRIX_LEN][PROB_MATRIX_LEN];
    for (int i = 0; i< w_len; i++){
        row_sum=0;
        for(int j = 0; j< w_len; j++){
            row_sum = prob_matrix[i][j] + row_sum;
            //printf("%lf",row_sum);
            prob_matrix_cdf[i][j] = row_sum;
            //printf("%lf",prob_matrix[i][j]);
        }
    }
    
    double rndm = rand()/(double)RAND_MAX;
    //printf("rndm %lf", rndm);
    for (int i = 0 ; i< w_len; i++){
        if(rndm <= prob_matrix_cdf[w_index][i]){
            //printf("w_space[i] %lf\n",w_space[i]);
            return(w_space[i]);
        }
    }
    
    return(-1);
}


int writeToFileXU(double x_array[2][arr_len], double u_array[arr_len], double w_array[arr_len], double t_array[arr_len], double V_array[arr_len],int exit_i){
    int i;
    FILE *outfile;
    char *outfilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_5D/XandU.txt";
    printf("%s",outfilename);
    if ((outfile = fopen(outfilename,"w")) == NULL)
    {
        printf("Error opening output file.\n");
        return(-1);
    }
    else{
        printf("\nfile opened. Output = t x[0] x[1] u\n");
    }
    fprintf(outfile,"%s %s %s %s %s %s %s %s\n","t","x0","x1","x2","x3","u","w","V_max");
    for(i=0;i<exit_i;i++)
    {
       fprintf(outfile,"%lf %lf %lf %lf %lf %lf %lf %lf\n",t_array[i],x_array[0][i], x_array[1][i], x_array[2][i], x_array[3][i], u_array[i], w_array[i], V_array[i]);
    }
    fclose(outfile);
    
    return 0;
}