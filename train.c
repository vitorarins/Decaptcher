#include "fann.h"

int main()
{
	printf("inicializando!\n");
	struct fann *ann = fann_create_standard(3, 1024, 512, 35);
	printf("fann created!\n");
        struct fann_train_data *fann_data = fann_read_train_from_file("./cnpj_dataset_32.data");
        fann_init_weights(ann,fann_data);
        fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	fann_train_on_data(ann, fann_data, 800, 10, 0.0000001);
	printf("fann trained!\n");
	fann_save(ann, "captcha_sigfunc_512n_800ep_32_cnpj.net");
	printf("fann saved!\n");
	fann_destroy(ann);
	return 0;
}
