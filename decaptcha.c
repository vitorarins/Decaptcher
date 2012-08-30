#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "fann.h"


int main(int argc, char* argv[])
{
	if(argc!= 2) error("Remember to specify an input file");
	
	struct fann *ann = fann_create_from_file("default-net-1024-500-32-1.0e-6.net");
	
	FILE *fp;
	char c;
	int i = 0;
	float bools[1024];
	while
	if(!(fp = fopen(argv[1],"r"))) error("Cannot open input file", argv[1]);
	while(i<1024){
	  c = fgetc(fp);
	  if(c == '0'){
	    bools[i]= 0;
	    i++;
	  }
	  if(c == '1'){
	    bools[i]= 1;
	    i++;
	  }
	}
	float *output = fann_run(ann, bools);
	std::cout << "1: " << output[0] << std::endl
		  << "3: " << output[1] << std::endl
		  << "5: " << output[2] << std::endl
		  << "6: " << output[3] << std::endl
		  << "7: " << output[4] << std::endl	  
		  << "8: " << output[5] << std::endl
		  << "A: " << output[6] << std::endl
		  << "B: " << output[7] << std::endl
		  << "C: " << output[8] << std::endl
		  << "D: " << output[9] << std::endl
		  << "E: " << output[10] << std::endl
		  << "F: " << output[11] << std::endl
		  << "G: " << output[12] << std::endl
		  << "H: " << output[13] << std::endl
		  << "I: " << output[14] << std::endl
		  << "J: " << output[15] << std::endl
		  << "K: " << output[16] << std::endl
		  << "L: " << output[17] << std::endl
		  << "M: " << output[18] << std::endl
		  << "N: " << output[19] << std::endl
		  << "O: " << output[20] << std::endl
		  << "P: " << output[21] << std::endl
		  << "Q: " << output[22] << std::endl
		  << "R: " << output[23] << std::endl
		  << "S: " << output[24] << std::endl
		  << "T: " << output[25] << std::endl
		  << "U: " << output[26] << std::endl
		  << "V: " << output[27] << std::endl
		  << "W: " << output[28] << std::endl
		  << "X: " << output[29] << std::endl
		  << "Y: " << output[30] << std::endl
		  << "Z: " << output[31] << std::endl;
	  
	return 0;
}
