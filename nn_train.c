#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "fann.h"

int main(int argc, char *argv[])
{
  int max_epochs_neurons;
  int hidden_layer_neurons;
  float erro;
  struct fann *ann;
  char *start = "captcha_classify_";
  char *end = ".net";
  char *filename = malloc(strlen(start)+strlen(argv[1])+strlen(end));
  if(argc ==3){
    max_epochs_neurons = atoi(argv[1]);
    erro = atof(argv[2]);
    strcat(filename, start);
    strcat(filename, argv[1]);
    strcat(filename, end);
    printf("inicializando!\n");
    ann = fann_create_shortcut(2, 1024, 35);
    printf("fann created!\n");
    fann_cascadetrain_on_file(ann, "./pgm_dataset.data", max_epochs_neurons, 10, erro);
    printf("fann trained!\n");
    fann_save(ann, filename);
    printf("fann saved!\n");
  }
  else if(argc == 5){
    hidden_layer_neurons = atoi(argv[1]);
    max_epochs_neurons = atoi(argv[2]);
    erro = atof(argv[3]);
    strcat(filename, start);
    strcat(filename, argv[1]);
    strcat(filename, end);
    printf("inicializando!\n");
    ann = fann_create_standard(3, 1024, hidden_layer_neurons, 35);
    printf("fann created!\n");
    fann_train_on_file(ann, argv[4], max_epochs_neurons, 10, erro);
    printf("fann trained!\n");
    fann_save(ann, filename);
    printf("fann saved!\n");
  }
  else{
    printf ("É preciso especificar 1º num de neurons in hidden layer e 2º num max de epochs 3º num erro, 4º nome do dataset\n");
    printf ("Ou no caso de cascade network 1º num max de neurons e 2º num erro!\n");
    return 1;
  }
  fann_destroy(ann);
  return 0;
}
