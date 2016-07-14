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



extern double prob_matrix[PROB_MATRIX_LEN][PROB_MATRIX_LEN]; // need to specify lengths for multi dim arrays
extern double del_t;
extern double u_space[];
extern double w_space[];
extern double x0_space[],x1_space[],x2_space[],x3_space[];
extern double x_min[];
extern double x_max[];
extern double step_size[];
extern double stopping_criterion;

extern double V[x1_len][x2_len][x2_len][x3_len][w_len];
extern double V_old[x1_len][x2_len][x2_len][x3_len][w_len];

double x_new[] = {0,0,0,0};
double x_old[] = {0,0,0,0};
double g = 1;
double temp_w[w_len];
double temp_u[u_len];
int max_temp_c = 0;

double max_of_V();
void deepcopy();
int writeToFile();
void parallel(int i0);

int value_iteration(){
    //double lol = prob_matrix1[0][0];
    //printf("%lf",lol);
    //init stuff
    
    
    
    
    
    
    
    
    
    
    int iteration = 0;
    int i0,i1,i2,i3,i4;
    
    for(i0=0;i0<x0_len;i0++){
        for(i1=0;i1<x1_len;i1++){
            for(i2=0;i2<x2_len;i2++){
                for(i3=0; i3<x3_len;i3++){
                    for(i4=0; i4<w_len; i4++){
                        V_old[i0][i1][i2][i3][i4] = 0;
                        V[i0][i1][i2][i3][i4] = 1;
                    }
                }
                
            }
        }
    }

    
    while(max_of_V() > stopping_criterion){
        deepcopy();
        
        for(i0=0;i0<x0_len;i0++){
            parallel(i0);
        }
     
        printf("iteration %d value %lf \n",iteration, max_of_V());
        
        iteration += 1;

    }
    
    printf("Value iteration Done\n Writing to file...\n");
    int status;
    status = writeToFile();
    printf("Done\n");
    
    return(0);
}

void parallel(int i0){
    for(int i1=0;i1<x1_len;i1++){
        for(int i2=0; i2<x2_len;i2++){
            for(int i3=0;i3<x3_len;i3++){
                for(int k=0; k<w_len;k++){
                    max_temp_c = 0;
                    for(int c=0;c<u_len;c++){
                        x_old[0] = x0_space[i0];
                        x_old[1] = x1_space[i1];
                        x_old[2] = x2_space[i2];
                        x_old[3] = x3_space[i3];
                        
                        //printf("1 %lf 2 %lf 3 %lf 4 %lf\n", x1_space[i], x2_space[j],u_space[c], w_space[k]);
                        pendulum_nonlinearmodel_ss(x_old, u_space[c], w_space[k],x_new);
                        //printf("non linear %lf %lf \n",x_new[0],x_new[1]);
                        temp_u[c] = g;
                        for(int d=0;d<w_len;d++){
                            temp_w[d] = interpol_5D(x_new[0], x_new[1], x_new[2],x_new[3], w_space[d]);
                            //printf("%lf temp_w[d]\n",temp_w[d]);
                            temp_u[c] = temp_u[c] + temp_w[d]*prob_matrix[k][d];
                            
                        }
                        //printf("temp u[c] %lf\n",temp_u[c]);
                        if(temp_u[c]>temp_u[max_temp_c]){
                            max_temp_c = c;
                        }
                        
                    }
                    
                    V[i0][i1][i2][i3][k] = temp_u[max_temp_c];
                    //printf("V[i][j][k] %lf \n",V[i][j][k]);
                    //printf("V_old[i][j][k] %lf \n",V_old[i][j][k]);
                    //printf("================\n");
                }
            }
            
        }
    }
}

int writeToFile(){
    int i0, i1, i2, i3, i4;
    FILE *outfile;
    char *outfilename = "/Users/Rounak/Desktop/Summer project/DCOC/DCOC_in_C/DCOC_5D/V_out.txt";
    printf("%s",outfilename);
    if ((outfile = fopen(outfilename,"w")) == NULL)
    {
        printf("Error opening output file.\n");
        return(-1);
    }
    else{
        printf("\nfile opened\n");
    }
    for(i0=0;i0<x0_len;i0++){
        for(i1=0;i1<x1_len;i1++){
            for(i2=0;i2<x2_len;i2++){
                for(i3=0; i3<x3_len;i3++){
                    for(i4=0; i4<w_len; i4++){
                        fprintf(outfile,"%lf\n",V[i0][i1][i2][i3][i4]);
                    }
                }
            }
        }
    }
    
    
    
    
    fclose(outfile);
    
    return 0;
}


double max_of_V(){
    int i0, i1, i2, i3, i4;
    double temp;
    double max = fabs(V[0][0][0][0][0]-V_old[0][0][0][0][0]);
    for(i0=0;i0<x0_len;i0++){
        for(i1=0;i1<x1_len;i1++){
            for(i2=0;i2<x2_len;i2++){
                for(i3=0; i3<x3_len;i3++){
                    for(i4=0; i4<w_len; i4++){
                        temp = fabs(V[i0][i1][i2][i3][i4]-V_old[i0][i1][i2][i3][i4]);
                        if(temp > max){
                            max = temp;
                        }
                    }
                }
            }
        }
    }
    return max;
}



void deepcopy(){
    int i0, i1, i2, i3, i4;
    for(i0=0;i0<x0_len;i0++){
        for(i1=0;i1<x1_len;i1++){
            for(i2=0;i2<x2_len;i2++){
                for(i3=0; i3<x3_len;i3++){
                    for(i4=0; i4<w_len; i4++){
                        V_old[i0][i1][i2][i3][i4] = V[i0][i1][i2][i3][i4];
                    }
                }
                
            }
        }
    }
}











int main(int argc, const char * argv[]) {
    
    int status;
    
    init_value_iteration();
    status = value_iteration();
    test_pendulum();
    return 0;
}


