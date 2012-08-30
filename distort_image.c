#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "captcha_headers.h"
#include "wand/MagickWand.h"
#include <string.h>

#define LOW -5.0
#define HIGH 5.0
#define MAXARGS 8

double randDouble()
{
  return rand()/((double)(RAND_MAX)+1);
}

double randDoubleRange(double min, double max)
{
  if(min>max)
    return randDouble()*(min-max)+max;
  else
    return randDouble()*(max-min)+min;
}

void blurImage(char *filename, char *outputname){
  MagickWand *wand;
  MagickBooleanType status;

  MagickWandGenesis();
  wand = NewMagickWand();
  status = MagickReadImage(wand, filename);
  if(status == MagickFalse)
    printf ("Erro ao ler imagem\n");
  MagickResizeImage(wand, 32, 32, BoxFilter, 0.0);

  MagickBlurImage(wand, 0, 1.0);
  MagickWriteImage(wand, path_cat(outputname, "blur.jpg"));

  DestroyMagickWand(wand);
}

void distortedImage(char *filename,char *outputname){
  int i;
  MagickWand *wand, *wand_cpy;
  MagickBooleanType status;
  double d_args[MAXARGS];
    
  //initrand();  
  for(i=0;i<MAXARGS;i++)
    d_args[i] = randDoubleRange(LOW,HIGH);
    
  MagickWandGenesis();
  wand = NewMagickWand();
  status = MagickReadImage(wand,filename);
  if(status == MagickFalse)
    printf("Erro ao ler imagem\n");
  MagickResizeImage(wand, 32, 32, BoxFilter, 0.0);
  
  /* wand_cpy = CloneMagickWand(wand); */
  /* MagickWaveImage(wand_cpy, d_args[0], MagickGetImageWidth(wand_cpy)); */
  /* MagickResizeImage(wand_cpy, 32, 32, BoxFilter, 0.0); */
  /* MagickWriteImage(wand_cpy, path_cat(outputname,"wave.jpg"));   */

  wand_cpy = CloneMagickWand(wand);
  MagickDistortImage(wand_cpy, BarrelDistortion,8,d_args, MagickTrue);
  MagickResizeImage(wand_cpy, 32, 32, BoxFilter, 0.0);
  MagickWriteImage(wand_cpy, path_cat(outputname,"barrel.jpg"));

  /* if(d_args[0] < 0){ */
  /*   wand_cpy = CloneMagickWand(wand); */
  /*   MagickImplodeImage(wand_cpy, d_args[0]); */
  /*   MagickResizeImage(wand_cpy, 32, 32, BoxFilter, 0.0); */
  /*   MagickWriteImage(wand_cpy, path_cat(outputname, "implode.jpg")); */
  /* } */
  
  wand_cpy = CloneMagickWand(wand);
  MagickDistortImage(wand_cpy, ScaleRotateTranslateDistortion,3,d_args, MagickTrue);
  MagickResizeImage(wand_cpy, 32, 32, BoxFilter, 0.0);
  MagickWriteImage(wand_cpy, path_cat(outputname,"srt.jpg"));

  wand_cpy = CloneMagickWand(wand);
  MagickDistortImage(wand_cpy, BilinearDistortion,4,d_args, MagickFalse);
  MagickResizeImage(wand_cpy, 32, 32, BoxFilter, 0.0);
  MagickWriteImage(wand_cpy, path_cat(outputname,"bilinear.jpg"));

  wand_cpy = DestroyMagickWand(wand_cpy);
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
}
