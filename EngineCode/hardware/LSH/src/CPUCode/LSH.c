
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

const float max_val = 1000000.0f;
const float cell_width = 1000.0f;

const unsigned int DIN = 2;
const unsigned int DOUT = 8;
const unsigned int REDUNDANT = 4;

// number of points
const int input_size = 10000;

float* input;

// center is the minimal value that is set as the zero point in DOUT space
float** center;
float** hash;
// for every input point, we output 2 uint64_t
int64_t** output_cpu;
int64_t** output_dfe;

void to_file();

void reserve_memory(){
	input = (float*) malloc( sizeof(float) * DIN * input_size);
	center = (float**) malloc( sizeof(float*) * REDUNDANT);
	hash = (float**) malloc( sizeof(float*) * DOUT);
	output_cpu = (int64_t**) malloc( sizeof(int64_t*) * REDUNDANT);
	output_dfe = (int64_t**) malloc( sizeof(int64_t*) * REDUNDANT);

	for(int i=0; i<REDUNDANT; i++){
		center[i] = (float*) malloc( sizeof(float) * DOUT);
		output_cpu = (int64_t*) malloc( sizeof(int64_t) * 2 * input_size);
		output_dfe = (int64_t*) malloc( sizeof(int64_t) * 2 * input_size);
	}

	for(int i=0; i<DOUT; i++)
		hash[i] = (float*) malloc( sizeof(float) * DIN);
}

void generate_data(){
	srand(0);
	// generate input
	int length = input_size * DIN;
	for(int i=0; i<length; i++)
		input[i] = float(rand()) / float(RAND_MAX) * max_val;
	
	// generate hash function and formalization
	for(int i=0; i<DOUT; i++)
		for(int j=0; j<DIN; j++)
			hash[i][j] = float(rand()) / float(RAND_MAX);
	for(int i=0; i<DOUT; i++){
		float sqr_sum = 0.0f;
		for(int j=0; j<DIN; j++)
			sqr_sum += hash[i][j] * hash[i][j];
		float sqrt_sum = sqrt(sqr_sum);
		for(int j=0; j<DIN; j++)
			hash[i][j]  /= sqrt_sum;
	}
	
	// generate center
	for(int red=0; red<REDUNDANT; red++){
		int rnd = rand() % input_size;
		for(int i=0; i<DOUT; i++){
			float mini = 0.0f;
			for(int j=0; j<DIN; j++)
				mini += input[rnd * DIN + j] * hash[i][j];
			center[red][i] = mini;
		}
	}

	// set output to be 0
	int output_length = input_size * DIN;
	for(int i=0; i<output_length; i++){
		output_cpu[i] = 0;
		output_dfe[i] = 0;
	}
}

void release_memory(){
	for(int i=0; i<REDUNDANT; i++){
		free(output_dfe[i]);
		free(output_cpu[i]);
		free(center[i]);
	}
	for(int i=0; i<DOUT; i++)
		free(hash[i]);

	free(hash);
	free(output_dfe);
	free(output_cpu);
	free(center);
	
	free(input);
}

void lsh_cpu(){
	float* temp = (float*) malloc(sizeof(float) * DOUT);
	float* mult = (float*) malloc(sizeof(float) * DOUT);
	for(int i=0; i<input_size; i++){
		// maek projection
		for(int j=0; j<DOUT; j++){
			float data = 0.0f;
			for(int k=0; k<DIN; k++)
				data += input[i * DIN + k] * hash[j][k];
			mult[j] = data;
		}
		// calculate index in each dimension
		for(int red=0; red<REDUNDANT; red++){
			for(int j=0; j<DOUT; j++)
				temp[j] = int((mult[j] - center[red][j]) / cell_width) + 1;
			// make the final hash
			int64_t first = 0;
			int64_t second = 0;
			for(int j=0; j<DOUT/2; j++){
				first += temp[j];
				first = first << 16;
			}
			for(int j=DOUT/2; j<DOUT; j++){
				second += temp[j];
				second = second << 16;
			}
			output_cpu[red][2 * i] = first;
			output_cpu[red][2 * i + 1] = second;
		}
	}
	free(temp);
	free(mult);
}

void lsh_dfe(){

}

void check_output(){
	int output_size = input_size * 2;
	int counter = 0;
	for(int i=0; i<output_length; i++)
		if(output_cpu[i] == output_dfe[i])
			counter++;
	printf("%d same over %d\n", counter, output_length);
	if(counter != output_length){
		printf("Error!!! please check files\n");
		to_file();
	}
}

void to_file(){
	int output_length = input_length * 2;
    FILE* fp1 = fopen("answer_cpu", "w");
    for(int i=0; i<output_length; i++)
        fprintf(fp1, "%ld\n", output_cpu[i]);
    fclose(fp1);
				
    FILE* fp2 = fopen("answer_dfe", "w");
    for(int i=0; i<output_length; i++)
        fprintf(fp2, "%ld\n", output_dfe[i]);
    fclose(fp2);
}

int main(void)
{
    printf("#########################################################################################\n");
    printf("Programming start\n");
    
	reserve_memory();
	generate_data();

	lsh_cpu();
	lsh_dfe();

	check_output();
	release_memory();

	printf("#########################################################################################\n");
    return 0;
}

