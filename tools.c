#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include "assert.h"
#include <math.h>


train_data get_one_hot_vectors(corpus c, int len_context, word_index word_to_index){
    train_data target_context_vector;
    int i;
    int j;
    int mot;
    target_context_vector.targets=(int **)malloc(c.nb_mots*sizeof(struct train_data));
    assert(target_context_vector.targets);
    target_context_vector.contexts=(int **)malloc(c.nb_mots*sizeof(struct train_data));
    assert(target_context_vector.contexts);
    target_context_vector.nb_one_context=(int*)malloc(c.nb_mots*sizeof(struct train_data));
    target_context_vector.nb_one_target=(int*)malloc(c.nb_mots*sizeof(struct train_data));
    for (i=0; i < c.nb_mots; i++){
        target_context_vector.targets[i]=(int *)malloc(word_to_index.nb_mots * sizeof(int));
        assert(target_context_vector.targets[i]);
        target_context_vector.contexts[i]=(int *)malloc(word_to_index.nb_mots * sizeof(int));
        assert(target_context_vector.contexts[i]);
    }   
    for (mot=0;mot<c.nb_mots;mot++){
        target_context_vector.nb_one_target[mot]=0;
        target_context_vector.nb_one_context[mot]=0;
        for(i=0;i<word_to_index.nb_mots;i++){
            if(strcmp(word_to_index.mots[i],c.mots[mot])==0){
                target_context_vector.targets[mot][word_to_index.index[i]]=1;
                target_context_vector.nb_one_target[mot]+=1;
            }
            else{
                target_context_vector.targets[mot][word_to_index.index[i]]=0;
            }
            target_context_vector.contexts[mot][word_to_index.index[i]]=0;
            if (mot==0){
                for(j=1;j<len_context+1;j++){
                    if(strcmp(word_to_index.mots[i],c.mots[j])==0){
                        target_context_vector.contexts[mot][word_to_index.index[i]]=1;
                        target_context_vector.nb_one_context[mot]+=1;
                    }
                }
            }
            else if (mot==c.nb_mots-1){
                    for(j=c.nb_mots-2;j>c.nb_mots-2-len_context;j--){
                        if(strcmp(word_to_index.mots[i],c.mots[j])==0){
                        target_context_vector.contexts[mot][word_to_index.index[i]]=1;
                        target_context_vector.nb_one_context[mot]+=1;
                        } 
                    }

            }
            else{
                for(j=mot-1;j>mot-1-len_context;j--){
                    if(j>=0){
                        if(strcmp(word_to_index.mots[i],c.mots[j])==0){
                        target_context_vector.contexts[mot][word_to_index.index[i]]=1;
                        target_context_vector.nb_one_context[mot]+=1;
                        }
                    }
                }
                for(j=mot+1;j<mot+1+len_context;j++){
                    if(j<c.nb_mots){
                        if(strcmp(word_to_index.mots[i],c.mots[j])==0){
                        target_context_vector.contexts[mot][word_to_index.index[i]]=1;
                        target_context_vector.nb_one_context[mot]+=1;
                        } 
                    }
                }
            
            }
        }
    }
    return target_context_vector;
}
            

    
word_index corpus_to_word_index(corpus c){
    word_index wi;
    wi.mots=malloc(c.nb_mots*sizeof(*c.mots));
    wi.index=malloc(c.nb_mots*sizeof(int));
    int i;
    int j;
    int index=0;
    int exist=0;
    size_t size;
    for (i=0;i<c.nb_mots;i++){
        exist=0;
        for (j=0;j<index;j++){
            if (strcmp(c.mots[i],wi.mots[j])==0){
                exist=1;
            }
        }
        if (exist==0){
            size=strlen(c.mots[i]);
            wi.mots[index]=(char*)malloc(size+1);
            wi.mots[index]=c.mots[i];
            wi.index[index]=index;
            index++;
            
        }
    }
    wi.nb_mots=index;
    return wi;
}


void forward_prop(weights *weights_input_hidden,weights *weights_hidden_output,int* target, double* hidden_layer, double* u, double* y_predicted){
    int i;
    int j;
    for (j=0;j<weights_input_hidden->nb_colonne;j++){
        for (i=0;i<weights_input_hidden->nb_ligne;i++){
            hidden_layer[j]+=weights_input_hidden->poid[i][j] * target[i];
        }
    }
    for (j=0;j<weights_hidden_output->nb_colonne;j++){
        for (i=0;i<weights_hidden_output->nb_ligne;i++){
            u[j]+=weights_hidden_output->poid[i][j] * hidden_layer[i];
        }
    }
    memcpy(y_predicted,u,sizeof(double)*weights_hidden_output->nb_colonne);
    softmax(y_predicted,weights_hidden_output->nb_colonne);
}


double* calculate_error(double* y_pred,int* context_words, int nb_one, int len){
    double* total_error=(double *) malloc(len*sizeof(double));
    int i; 
    for(i=0; i<len;i++){        
        if (context_words[i]==1){
            total_error[i]= (y_pred[i]-1)+ ((nb_one -1)*y_pred[i]);
        }
        else{
            total_error[i]=nb_one * y_pred[i];        
        }
    }
    return total_error;
}

void softmax(double* input, int size) {
	int i;
	double m, sum, constant;
	m = -INFINITY;
	for (i = 0; i < size; ++i) {
		if (m < input[i]) {
			m = input[i];
		}
	}
	sum = 0.0;
	for (i = 0; i < size; ++i) {
		sum += exp(input[i] - m);
	}
	constant = m + log(sum);
	for (i = 0; i < size; ++i) {
		input[i] = exp(input[i] - constant);
	}

}

void backward_prop(weights *weights_hidden_input,weights *weights_hidden_output, double*  total_error, double* hidden_layer, int* target_word_vector, double learning_rate){
    double* tmp=calloc(weights_hidden_output->nb_ligne,sizeof(double));
    assert(tmp);
    int i, j; 
    for(i=0;i<weights_hidden_output->nb_ligne;i++){
        for(j=0;j<weights_hidden_output->nb_colonne;j++){
            tmp[i]= tmp[i] + weights_hidden_output->poid[i][j]*total_error[j];
        }
    }

    for(i=0;i<weights_hidden_input->nb_ligne;i++){
        for(j=0;j<weights_hidden_input->nb_colonne;j++){
            weights_hidden_input->poid[i][j]=weights_hidden_input->poid[i][j]-(learning_rate*(target_word_vector[i]*tmp[j]));
        }
    }
    for(i=0; i<weights_hidden_output->nb_ligne;i++){
        for(j=0;j<weights_hidden_output->nb_colonne;j++){
            weights_hidden_output->poid[i][j]=weights_hidden_output->poid[i][j]-(learning_rate*(hidden_layer[i]*total_error[j]));
        }
    }
    
}

double calculate_loss(double* u,int* context_vector, int len){
    double total_loss=0.0, sum_1=0.0, sum_2=0.0;
    int i, tmp=0; 
    for(i=0;i<len;i++){
        if (context_vector[i]==1){
            sum_1=sum_1+u[i];
            tmp+=1;
        }
        sum_2+=exp(u[i]);
    }

    sum_1=-sum_1;
    sum_2=tmp * log(sum_2);
    total_loss = sum_1 + sum_2;
    return total_loss;
}