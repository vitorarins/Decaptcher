#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "captcha_headers.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAXFILES 100000
#define N_DISTORTIONS 10

/* Initializes the rand() function with the time on the clock */
void initrand()
{
  time_t seconds;

  time(&seconds);
  srand((unsigned int) seconds);
}


void gen_random(char *s, const int len) {
  int i;
     for ( i = 0; i < len; ++i) {
         int randomChar = rand()%(26+26+10);
         if (randomChar < 26)
             s[i] = 'a' + randomChar;
         else if (randomChar < 26+26)
             s[i] = 'A' + randomChar - 26;
         else
             s[i] = '0' + randomChar - 26 - 26;
     }
     s[len] = 0;
 }

/* The function generateDistortImages will create a directory inside dirname, and */
/* generate distorted images with the function distortedImage() inside the */
/* 'distort_image.c' file. To get the images, it uses the function */
/* runThruDirectories() from the 'gera_data_set.c' file, which returns the paths */
/*   to the Images. */
void generateDistortImages(char *dirname)
{
  char *filepaths[MAXFILES], directories[MAXFILES], randString[60];
  int i, j, records;
  struct stat s;

  records = runThruDirectories(dirname, filepaths, directories);
  initrand();
  char *output_folder = path_cat(dirname,"distort_images/");
  if(mkdir(output_folder, S_IRWXU) != 0)
    printf ("Not able to create directory\n");

  
  for(i=0;i<records;i++){
    char *temp = malloc(3);
    sprintf(temp,"%c/",directories[i]);
    char *output = path_cat(output_folder, temp);
    if(stat(output, &s) != 0){
      if(mkdir(output, S_IRWXU) != 0)
        printf ("Not able to create directory\n");
    }
    
    for(j=1;j<=N_DISTORTIONS;j++){
      gen_random(randString, 50);
      char *output_name = path_cat(output,randString);
      distortedImage(filepaths[i],output_name);
    }
    /* blurImage(filepaths[i], path_cat(output, randString)); */
    free(filepaths[i]);
  }
}

int main(int argc, char *argv[])
{
  generateDistortImages(argv[1]);

  return 0;
}
