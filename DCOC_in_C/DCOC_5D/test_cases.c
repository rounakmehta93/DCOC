//
//  test_test_pendulum.c
//  DCOC_in_C
//
//  Created by Rounak on 13/7/16.
//  Copyright © 2016 Rounak. All rights reserved.
//
#define x0_len 3
#define x1_len 3
#define x2_len 3
#define x3_len 3
#define w_len 1
#include <stdio.h>
#include <stdlib.h>
int writeToFile();
void main(){
	writeToFile();
}

double V[x0_len][x1_len][x2_len][x3_len][w_len] = {{{{{123244},{2},{3}},{{1},{2},{3}},{{1},{2},{3}}},
		{{{1},{2},{3}},{{4545},{2},{3}},{{1},{2},{3}}},
		{{{1},{2},{3}},{{1},{2},{3}},{{1},{2},{3}}}},
		{{{{1},{2},{3343}},{{1},{24545},{3}},{{1},{2},{23231}}},
		{{{1},{2},{3}},{{1},{2},{98}},{{1},{2},{3}}},
		{{{1},{34345},{3}},{{1},{2},{3}},{{1},{32},{3}}}},
		{{{{1},{2},{4543}},{{1},{2},{554}},{{1},{2},{3}}},
		{{{1},{2},{3}},{{1},{2},{3}},{{1},{212},{3}}},
		{{{1},{2},{3}},{{1},{998982},{3}},{{100},{2},{9293}}}}};




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
    printf("%lf %lf %lf %lf",V[0][0][0][0][0],V[2][1][0][2][0],V[2][2][2][2][0], V[1][1][2][2][0]);
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
}