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
    printf ("%c: %f\n",out,output[i]);
  }


  *letra = labels[letnum];

  /* if( letnum < 9) */
  /*   *letra = letnum + '1'; */
  /* else */
  /*   *letra = letnum - 9 + 'a'; */
  /* else */
  /*   *letra = letnum - 36 + 'A'; */

}

/* The path_cat Function concatenates the path we already have to the path */
/* of the folder we want to open. */
char *path_cat (const char *str1, char *str2) {
  size_t str1_len = strlen(str1);
  size_t str2_len = strlen(str2);
  char *result;
  result = malloc((str1_len+str2_len+1)*sizeof *result);
  strcpy (result,str1);
  int i,j;
  for(i=str1_len, j=0; ((i<(str1_len+str2_len)) && (j<str2_len));i++, j++) {
    result[i]=str2[j];
  }
  result[str1_len+str2_len]='\0';
  return result;
}

/* The runThruDirectories Function finds every image in the dirname Directory */
/* and feeds the string(char *) array files with the path to the images. */
/* It returns the number of files that were found. */

int runThruDirectories(char *dirname, char *files[], char char_directory[])
{
  unsigned char isFolder =0x4;
  /* a DIR variable is needed for dirname and the character directory
  * The same thing for the dirent structure */
  DIR             *dip, *letra;
  struct dirent   *dit, *letras;
  int i=0;

  /* DIR *opendir(const char *name);
   * Open a directory stream to dirname and make sure
   * it's a readable and valid (directory) */
  if ((dip = opendir(dirname)) == NULL)
    {
      perror("opendir");
    }
 
  printf("Directory stream is now open\n");

  /*  struct dirent *readdir(DIR *dir);
   * Read in the files from character directory and print */
  while((dit = readdir(dip)) != NULL){

    char *filepath;
    
    if(strcmp(dit->d_name,"..") == 0 || strcmp(dit->d_name, ".svn") == 0)
      continue;
    
    filepath = path_cat(dirname, dit->d_name);
    strcat(filepath,"/");
    
    if(( letra = opendir(filepath)) == NULL) /* opens character Directory */
      perror("opendir");

    while ((letras = readdir(letra)) != NULL)
      {
        if(letras->d_type == isFolder)
          continue;
        files[i] = path_cat(filepath, letras->d_name);
        char_directory[i] = dit->d_name[0];
        i++;
      }
    free(filepath);
    filepath = NULL;
    if(closedir(letra) == -1)
      perror("closedir");
  }

   /* int closedir(DIR *dir);
   * Close the stream to dirname. And check for errors. */
  if (closedir(dip) == -1)
    {
      perror("closedir");
    }
  printf("\nDirectory stream is now closed\n");

  return i;  
}

/*The Function testNeuralNet consumes a directory string(char *) dirname,to*/
/* runThruDirectories. After taking the images filepaths, the function converts */
/*each one to a float array and runs a neural net named nnfilename with the array*/
/* to tell the right letter.  */
void testNeuralNet(char *dirname, char *nnfilename)
{
  char *filepaths[MAXFILES], directories[MAXFILES];
  char *letra_certa = malloc(1);
  int i, records;
  double erros, acertos, total, pcent;
  erros = acertos = pcent = total = 0.0;

  /* create artificial neural network from nnfilename */
  struct fann *ann = fann_create_from_file(nnfilename);
  
  records = runThruDirectories(dirname, filepaths, directories);
  
  for(i=0;i<records;i++){
    float imgfloat[IMAGEPIXELS];
    vetorImagem(filepaths[i], imgfloat);
    float *output = fann_run(ann, imgfloat);
    letraCerta(letra_certa, output, NUMDIRETORIES);
    printf ("letra_certa %c\n",*letra_certa);
    printf ("letra_diretorio %c\n",directories[i]);
    if(*letra_certa == directories[i])
      acertos++;
    else
      erros++;
  }
  total = (double) records;
  pcent = (acertos * 100)/total;
  printf ("A rede teve %.0f acertos e %.0f erros!\n",acertos, erros);
  printf ("A rede teve %.2f%% de acerto!\n",pcent);
  
}

int main(int argc, char *argv[])
{
  if(argc < 3)
    printf ("É preciso especificar o diretório e em seguida o nome da rede!\n");

  testNeuralNet(argv[1], argv[2]);
  /* char *let=malloc(2); */
  /* float out[NUMDIRETORIES] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,.0}; */
  /* letraCerta(let,out,NUMDIRETORIES); */
  /* printf ("%s\n",let); */
  /* printf ("%d\n",*let); */

  return 0;
}

