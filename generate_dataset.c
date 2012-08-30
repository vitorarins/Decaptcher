#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAXFILES 600000
#define IMAGEPIXELS 1024
#define NUMDIRETORIES 34

/*Function to generate a BitArray for the image classification!
*It feeds the array with '0's or '1' depending on the character
* given through dir_char, which is the char of the directory the image is in*/
void geraBitArray(char right_char_array[], char dir_char, int limit)
{
  int i;
  char labels[] = "123456789abcdefghijklmnpqrstuvwxyz";

  for(i=0; i < limit; i++){
    if(labels[i] == dir_char)
      right_char_array[i] = '1';
    else
      right_char_array[i] = '0';
  }
  right_char_array[NUMDIRETORIES]='\0';
  /* for(k=j; k<limit; k++){ // for getting capitol letters */
  /*   if((k - j) + 'A' == dir_char) */
  /*     right_char_array[k] = '1'; */
  /*   else */
  /*     right_char_array[k] = '0'; */
  /* } */
}

/* The path_cat Function concatenates the path we already have to the path */
/* of the folder we want to open. */
char *path_cat (const char *str1, char *str2) {
  size_t str1_len = strlen(str1);
  size_t str2_len = strlen(str2);
  char *result;
  result = malloc((str1_len+str2_len+1)*sizeof(char) );
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
  char *filepath;  

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


    if(strcmp(dit->d_name,"..") == 0 || strcmp(dit->d_name, ".svn") == 0)
      continue;
    filepath = path_cat(dirname,path_cat(dit->d_name,"/"));

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

/*The Function generateDatasetFile consumes a directory string(char *) dirname,to*/
/* runThruDirectories. After taking the images filepaths, the function converts */
/*each one to a float array and writes the array to a file named (char *)filename*/
/* saved into dirname.   */
void generateDatasetFile(char *dirname, char *filename)
{
  FILE *output_file;
  char *filepaths[MAXFILES], directories[MAXFILES], right_char[NUMDIRETORIES];
  char recordstring[4], pixels[4], numdirs[2];
  char output_name[strlen(dirname)+strlen(filename)];
  int i, j, records;

  sprintf(pixels,"%d",IMAGEPIXELS);
  sprintf(numdirs,"%d",NUMDIRETORIES);

  strcpy(output_name, dirname);
  strcat(output_name, filename);
  records = runThruDirectories(dirname, filepaths, directories);
  sprintf(recordstring,"%d",records);

  output_file = fopen(output_name, "w");
  if(output_file == NULL)
    printf ("Can't open output file!\n");

  fprintf(output_file,"%s %s %s\n",recordstring,pixels,numdirs);
  for(i=0;i<records;i++){
    float imgfloat[IMAGEPIXELS];
    vetorImagem(filepaths[i], imgfloat);
    for (j = 0; j < IMAGEPIXELS; ++j)
      fprintf(output_file,"%.3f ",imgfloat[j]);
    geraBitArray(right_char, directories[i], NUMDIRETORIES);
    fprintf(output_file, "\n");
    for(j=0;j < NUMDIRETORIES; j++)
      fprintf(output_file,"%c ", right_char[j]);
    fprintf(output_file,"\n");
    
  }
  fclose(output_file);
}

int main(int argc, char *argv[])
{
  /* CU_ErrorCode resultado = CU_initialize_registry(); */
  /* if(resultado == CUE_NOMEMORY) */
  /* printf ("It wasn't possible to allocate memory for the registry\n"); */
  
  if(argc < 3)
    printf ("É preciso especificar o diretório das imagens e o nome do arquivo de saída!\n");
  /* char r[NUMDIRETORIES]; */
  /* geraBitArray(r,'1',NUMDIRETORIES); */
  /* printf("%s\n",r); */

  generateDatasetFile(argv[1], argv[2]);
  
  /* CU_cleanup_registry(); */
  
  return 0;
  
}
