#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "captcha_headers.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "fann.h"

#define MAXFILES 50000
#define IMAGEPIXELS 1024
#define NUMDIRETORIES 34

/*Function to tell the right letter for the image classification!
*It feeds the array with '0's or '1' depending on the character*/
void letraCerta(char *letra, float *output, int limit)
{
  int i, letnum;
  char out;
  float maior = 0.0;
  char labels[] = "123456789abcdefghijklmnpqrstuvwxyz";

  for(i=0; i<limit; i++){
    if(output[i] > maior){
      maior = output[i];
      letnum = i;
    }
    out = labels[i];
    /* printf ("%c: %f\n",out,output[i]); */
  }


  *letra = labels[letnum];

  /* if( letnum < 9) */
  /*   *letra = letnum + '1'; */
  /* else */
  /*   *letra = letnum - 9 + 'a'; */
  /* else */
  /*   *letra = letnum - 36 + 'A'; */

}

/*The Function testNeuralNet consumes a directory string(char *) dirname,to*/
/* runThruDirectories. After taking the images filepaths, the function converts */
/*each one to a float array and runs a neural net named nnfilename with the array*/
/* to tell the right letter.  */
double testNeuralNetOnData(char *ds_filename, struct fann *ann)
{
  char *letra_certa = malloc(1);
  char *letra_output = malloc(1);
  int i, j, records, pixels, output_numbers;
  float imgfloat[IMAGEPIXELS], letra_float[NUMDIRETORIES];
  double erros, total, pcent;
  acertos = pcent = total = 0.0;
  FILE *data_file = fopen(ds_filename, "r");

  fscanf(data_file,"%d %d %d", &records, &pixels, &output_numbers);
  for(i=0;i<records;i++){
    for(j=0;j<pixels;j++)
      fscanf(data_file,"%f", &imgfloat[j]);
    for(j=0;j<output_numbers;j++)
      fscanf(data_file,"%f",&letra_float[j]);
    letraCerta(letra_certa, letra_float, NUMDIRETORIES);
    float *output = fann_run(ann, imgfloat);
    letraCerta(letra_output, output, NUMDIRETORIES);
    if(*letra_certa != *letra_output)
      erros++;
  }
  fclose(data_file);
  total = (double) records;
  pcent = (erros * 100)/total;
  pcent = pcent/100;
  
  return pcent;
}

/* int main(int argc, char *argv[]) */
/* { */
/*   if(argc < 3) */
/*     printf ("You need to specify the test dataset and the network!"); */

/*   testNeuralNetOnData(argv[1], argv[2]); */
/*   /\* char *let=malloc(2); *\/ */
/*   /\* float out[NUMDIRETORIES] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,.0}; *\/ */
/*   /\* letraCerta(let,out,NUMDIRETORIES); *\/ */
/*   /\* printf ("%s\n",let); *\/ */
/*   /\* printf ("%d\n",*let); *\/ */

/*   return 0; */
/* } */

