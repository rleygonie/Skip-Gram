
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include "assert.h"


int main(){
    int epoch=5;
    corpus c;
    char* path=malloc(10000*sizeof(char));
    printf("Veuillez entrer le chemin d'accès au corpus d'entraînement :\n");
    scanf("%s", path);
    c=lire_fichier(0,path);
    word_index wi;
    wi = corpus_to_word_index(c);
    train_data train;
    train=get_one_hot_vectors(c, 2, wi);

    post_train *t;
    printf("---------------------------Start training--------------------------\n");
    t=training(50,epoch,train,0.01,1 ,20, wi, c);
    printf("----------------------------End training---------------------------\n");
    word_similarity *ws;
    char* mot=malloc(100*sizeof(char));
    do {
            do{
            printf("\nVeuillez entrer un mot du corpus :\nPour arrêter, entrez 'stop'\n");
            scanf("%s", mot);
            ws=cosine_similarity(mot,t->w_1[epoch-1],wi);
        }while (ws->index==-1);
        if (strcmp(mot,"stop")!=0){
            int n=0;
            printf("Combien de mots similaires à celui que vous venez d'entrer souhaitez-vous connaître? :\n");
            scanf("%d", &n);
            top_n_smilarity(mot,ws,n,wi.nb_mots);
        }
    }while(strcmp(mot,"stop")!=0);
    return 0;
}