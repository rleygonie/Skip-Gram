#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <time.h> 
#include <ctype.h>
#include "header.h"
#include "assert.h"


post_train* training(int word_embedding_dimension,int epochs,train_data train,double learning_rate,int disp ,int interval, word_index wi, corpus c){
    weights *w_input;
    weights *w_output;
    w_input=initialisation_hidden(wi.nb_mots,word_embedding_dimension,1,-1,1);
    w_output=initialisation_hidden(wi.nb_mots,word_embedding_dimension,0,-1,1);

    post_train *t=malloc(sizeof(*t));

    t->epoch_loss=malloc(epochs*sizeof(double));
    t->w_1=malloc(epochs*sizeof(weights*));
    t->w_2=malloc(epochs*sizeof(weights*));

    int i, j,a;
    double loss_tmp, loss;
    time_t begin =time(NULL);
    for(i=0;i<epochs;i++){
        loss=0.0;
        for(j=0; j<c.nb_mots;j++){
            double* total_error=calloc(w_output->nb_colonne,sizeof(double));
            double* hidden_layer=calloc(w_input->nb_colonne,sizeof(double));
            double* u=calloc(w_output->nb_colonne,sizeof(double));
            double* y_pred=calloc(w_output->nb_colonne,sizeof(double));
    
            forward_prop(w_input,w_output,train.targets[j], hidden_layer, u, y_pred);
            total_error= calculate_error(y_pred,train.contexts[j], train.nb_one_context[j], wi.nb_mots);
            backward_prop(w_input,w_output,total_error,hidden_layer,train.targets[j], learning_rate);
            loss_tmp = calculate_loss(u,train.contexts[j],wi.nb_mots);
            loss = loss + loss_tmp;

            free(hidden_layer);
            free(u);
            free(y_pred);
            }

        t->epoch_loss[i]=loss;
        t->w_1[i].poid=(double**)malloc(wi.nb_mots*sizeof(double*));
        t->w_1[i].nb_ligne=wi.nb_mots;
        t->w_1[i].nb_colonne=word_embedding_dimension;
        
        for (a=0;a<wi.nb_mots;a++){
            t->w_1[i].poid[a]=(double*)malloc(word_embedding_dimension*sizeof(double));
            memcpy(t->w_1[i].poid[a],w_input->poid[a],sizeof(double)*word_embedding_dimension);
        }

        t->w_2[i].poid=(double**)malloc(wi.nb_mots*sizeof(double*));
        t->w_2[i].nb_ligne=word_embedding_dimension;
        t->w_2[i].nb_colonne=wi.nb_mots;
        for (a=0;a<word_embedding_dimension;a++){
            t->w_2[i].poid[a]=(double*)malloc(wi.nb_mots*sizeof(double));
            memcpy(t->w_2[i].poid[a],w_output->poid[a],sizeof(double)*wi.nb_mots);
        }
        if (disp==1){
            if (i==0 || i%interval==0 || i==i-1){
                printf("Epoch : %d   Loss : %f  \n", i, loss);
            }
        }
    }
    time_t end = time(NULL);
    unsigned long secondes = (unsigned long ) difftime(end, begin);
    printf("L'apprentissage à duré : %ld secondes \n", secondes);

    return t;
}






