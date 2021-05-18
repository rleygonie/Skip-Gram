#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include "assert.h"


corpus lire_fichier(int stop_word,char* path){
    char stop_words[]="me my myself we our ours ourselves you your yours yourself yourselves he him his himself she her hers herself it its itself they them their theirs themselves what which who whom this that these those am is are was were be been being have has had having do does did doing an the and but if or because as until while of at by for with about against between into through during before after above below to from up down in out on off over under again further then once here there when where why how all any both each few more most other some such no nor not only own same so than too very can will just don should now";
    corpus b;
    FILE *texte;
    int c;
    texte = fopen(path , "r");
    b.nb_mots=0;
    FILE *fichier2 = NULL;
    fichier2 = fopen("corpus.txt", "w");
    while ((c = getc(texte)) != EOF){
        if (isalpha(c)){
            fputc(tolower(c), fichier2);
        }
        if (c==' '){
            b.nb_mots+=1;
            fputc(c, fichier2);
        }
    }
    fclose(fichier2);
    fclose(texte);
    //Allocation de memoire
    b.mots=malloc(b.nb_mots*sizeof(*b.mots));
    assert(b.mots);

    FILE *corpus;
    corpus = fopen("corpus.txt" , "r");
    char *str;
    char * d=" ";
    char * line = (char *) malloc(600);
    int i=0;
    size_t size;
    
    // Si stopword=1, ils ne seront pas pris en compte dans l'apprentissagen et inversement 
    if (stop_word==1){
        while (!feof(corpus)) {
            fgets (line, 600, corpus);
            str = line;
            char * p = strtok(str, d);
            char* result;
            while(p != NULL){
                result = strstr(stop_words, p);
                if (result == NULL) {
                    size=strlen(p);
                    b.mots[i]=(char*)malloc(size+1);
                    strcpy(b.mots[i],p);
                    i++;
                }
                p = strtok(NULL, d);
            }
    }

    }
    else{
        while (!feof(corpus)) {
            fgets (line, 600, corpus);
            str = line;
            char * p = strtok(str, d);
            while(p != NULL){
                size=strlen(p);
                b.mots[i]=(char*)malloc(size+1);
                strcpy(b.mots[i],p);
                p = strtok(NULL, d);
                i++;
            }
        }
    }
    b.nb_mots=i;
    fclose(corpus);
    return b;

}


weights* initialisation_hidden(int vocab_size, int dimension, int input, int min, int max){
    weights *w=malloc(sizeof(*w));
    int i; 
    int j; 
    double prob;
    prob =(double) 1/(max-min);
    //w_input
    if (input==1){
        w->nb_ligne=vocab_size;
        w->nb_colonne=dimension;
        w->poid=(double**)malloc(vocab_size*sizeof(double*));
        for (i=0;i<vocab_size;i++){
            w->poid[i]=(double*)malloc(dimension*sizeof(double));
            for (j=0;j<dimension;j++){
                w->poid[i][j]=(rand()/(double)RAND_MAX*(max-min)+min) * prob;
            }
        }
    }
    //w_output
    else {
        w->nb_ligne=dimension;
        w->nb_colonne=vocab_size;
        w->poid=(double**)malloc(dimension*sizeof(double*));
        for (i=0;i<dimension;i++){
            w->poid[i]=(double*)malloc(vocab_size*sizeof(double));
            for (j=0;j<vocab_size;j++){
                w->poid[i][j]=(rand()/(double)RAND_MAX*(max-min)+min) * prob;
            }
        }
    }
    return w;
}





