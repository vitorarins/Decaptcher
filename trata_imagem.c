#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* #include "captcha_headers.h" */
#include "wand/MagickWand.h"

void vetorImagem(char *filename,float *imgfloat){
  unsigned char *imgchar;
  int i;
  MagickWand *wand;
  MagickBooleanType status;
  size_t blobsize;
  
  MagickWandGenesis();
  wand = NewMagickWand();
  status = MagickReadImage(wand,filename);
  if(status == MagickFalse)
    printf("Erro ao ler imagem\n");
  //MagickBlurImage(wand, 0.0, 1.0);
  /* MagickResizeImage(wand, 16,16, BoxFilter, 0.0); */
  /* MagickThresholdImage(wand,40000); */
  MagickSetFormat(wand, "PBM");
  MagickSetCompression(wand, NoCompression);
  /* MagickWriteImage(wand, "letra.pgm"); */
  
    
  imgchar = MagickGetImageBlob(wand, &blobsize);
  imgchar += 8;
  
  for(i=0;i<blobsize-8;i++){
    if(*imgchar == '1'){
      *imgfloat = 0;
      imgchar++;
      imgfloat++;
    }
    else if(*imgchar == '0'){
      *imgfloat = 1;
      imgchar++;
      imgfloat++;
    }
    else
      imgchar++;
  }

  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
} 

/* int main(int argc, char* argv[]){ */
/*   float imagebin[1024]; */
/*   int j; */
/*   FILE *fp; */
/*   if(!(fp = fopen("/home/vitor/Documentos/Projetos/Captcha/letra.txt","w"))) printf ("Erro ao abrir arquivo!\n"); */
/*   vetorImagem(argv[1], imagebin); */
/*   for(j=0;j<1024;j++) */
/*     fprintf(fp,"%.0f ",imagebin[j]); */

/*   fprintf(fp,"\n"); */
/*   fclose(fp); */

/*   return 0; */
/* } */
