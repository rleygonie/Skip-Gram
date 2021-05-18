
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



struct corpus{
    char** mots;
    int nb_mots;
};typedef struct corpus corpus;

struct word_index{
    char** mots;
    int* index;
    int nb_mots;
};typedef struct word_index word_index;

struct train_data{
    int** targets;
    int** contexts;
    int* nb_one_target;
    int* nb_one_context;
};typedef struct train_data train_data;

struct weights{
  unsigned int nb_ligne;
  unsigned int nb_colonne;
  double** poid;
};typedef struct weights weights;

struct post_train{
    struct weights* w_1;
    struct weights* w_2;
    double* epoch_loss;
}; typedef struct post_train post_train;

struct word_similarity{
    char** mots;
    double* theta;
    int index;
}; typedef struct word_similarity word_similarity;

train_data get_one_hot_vectors(corpus c, int len_context,word_index word_to_index);
weights* initialisation_hidden(int vocab_size, int dimension, int input, int min, int max);
corpus lire_fichier(int stop_word,char* path);
word_index corpus_to_word_index(corpus c);
void softmax(double* input, int size);
void forward_prop(weights *weights_input_hidden,weights *weights_hidden_output,int* target, double* hidden_layer, double* u, double* y_predicted);
double* calculate_error(double* y_pred,int* context_words, int nb_one, int len);
void backward_prop(weights *weights_hidden_input,weights *weights_hidden_output, double*  total_error, double* hidden_layer, int* target_word_vector, double learning_rate);
double calculate_loss(double* u,int* context_vector, int len);
post_train* training(int word_embedding_dimension,int epochs,train_data train,double learning_rate,int disp ,int interval, word_index wi, corpus c);
word_similarity* cosine_similarity(char* word,weights weight,word_index wi);
void* top_n_smilarity(char* word,word_similarity *ws,int n,int vocab_size);
