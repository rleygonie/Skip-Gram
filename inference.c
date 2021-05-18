#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include "assert.h"




word_similarity* cosine_similarity(char* word,weights weight,word_index wi){
    word_similarity *ws=malloc(sizeof(*ws));
    assert(ws);
    ws->mots=(char**)malloc(wi.nb_mots*sizeof(char*));
    assert(ws->mots);
    ws->theta=(double*)malloc(wi.nb_mots*sizeof(double));
    assert(ws->theta);
    double* vector_1=(double*)calloc(weight.nb_colonne,sizeof(double));
    double* vector_2=(double*)calloc(weight.nb_colonne,sizeof(double));
    assert(vector_1);
    assert(vector_2);
    int i,j,taille,index=-1;
    double theta_sum, theta_den_1, theta_den_2, theta_den, theta;
    for(i=0;i<wi.nb_mots;i++){
        if (strcmp(wi.mots[i],word)==0){
            index=i;
        }

        
    }
    ws->index=index;
    if (index==-1){
        printf("\nLe mot entré n'existe pas dans le corpus ! Le modèle ne le connaît pas :( \n\n ");
    }
    else {
        memcpy(vector_1,weight.poid[index],sizeof(double)*weight.nb_colonne);
        for (i=0;i<wi.nb_mots;i++){

            theta_sum=0.0;
            theta_den_1=0.0;
            theta_den_2=0.0;
            theta_den=0.0;
            memcpy(vector_2,weight.poid[i],sizeof(double)*weight.nb_colonne);
            for(j=0;j<weight.nb_colonne;j++){
                theta_sum+=vector_1[j]*vector_2[j];
                theta_den_1+=pow(vector_1[j],2);
                theta_den_2+=pow(vector_2[j],2);
            }
            theta_den=sqrt(theta_den_1)*sqrt(theta_den_2);
            theta= (double)theta_sum/(double)theta_den;

            taille=strlen(wi.mots[i]);
            ws->mots[i]=(char*)malloc(taille+1);
            assert(ws->mots[i]);
            strcpy(ws->mots[i],wi.mots[i]);
            ws->theta[i]=theta;

        }
    }
    return ws;
}

void* top_n_smilarity(char* word,word_similarity *ws,int n,int vocab_size){
    int index[n];
    int i,j,z,exist;
    double max;
    printf("Mot : %s\n ", word);
    for(i=0;i<n;i++){
        max=0.0;
        for(j=0;j<vocab_size;j++){
            if (ws->theta[j]>max && ws->theta[j]<0.9999999999){
                exist=0;
                for(z=0;z<i;z++){
                    if (index[z]==j){
                        exist=1;
                    }
                }
                if (!exist){
                    index[i]=j;
                    max=ws->theta[j];
                }

            }
        }
    }
    for(i=0;i<n;i++){
        printf("Similarité n° %d : \"%s\" -> %f \n ",i+1,ws->mots[index[i]],ws->theta[index[i]]);
    }
    return 0;
}

        
