#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "fann.h"

int main(void)
{

  const unsigned int num_input = 32 * 32;
  const unsigned int num_output = 35;
  const unsigned int num_layers = 3;
  const unsigned int num_hidden = 1000;
  const float desired_error = (const float) 0.0027;
  const unsigned int max_epochs = 10000;
  const unsigned int epochs_between_reports = 100;
  struct fann *ann;

  ann = fann_create_standard (num_layers, num_input, num_hidden, num_output);
  fann_set_activation_function_hidden (ann, FANN_SIGMOID_SYMMETRIC);
  fann_set_activation_function_output (ann, FANN_SIGMOID_SYMMETRIC);
  fann_randomize_weights (ann, -0.01, 0.0);

  fann_train_on_file(ann, "captcha-ds.data", max_epochs, epochs_between_reports, desired_error);
  fann_save(ann, "rede_randomweights_500_1.0028.net");

  fann_destroy (ann);

  return 1;
}
