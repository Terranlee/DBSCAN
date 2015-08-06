
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
const int input_size = 1000;

float* input;

// center is the minimal value that is set as the zero point in DOUT space
float** center;
float** hash;
// for every input point, we output 2 uint64_t
int64_t** output_cpu;
int64_t** output_dfe;

int16_t** middle_result_dfe;
int16_t** middle_result_cpu;

void to_file();

void reserve_memory(){
	printf("reserve memory\n");
	input = (float*) malloc( sizeof(float) * DIN * input_size);
	center = (float**) malloc( sizeof(float*) * REDUNDANT);
	hash = (float**) malloc( sizeof(float*) * DOUT);
	output_cpu = (int64_t**) malloc( sizeof(int64_t*) * REDUNDANT);
	output_dfe = (int64_t**) malloc( sizeof(int64_t*) * REDUNDANT);

	for(int i=0; i<REDUNDANT; i++){
		center[i] = (float*) malloc( sizeof(float) * DOUT);
		output_cpu[i] = (int64_t*) malloc( sizeof(int64_t) * 2 * input_size);
		output_dfe[i] = (int64_t*) malloc( sizeof(int64_t) * 2 * input_size);
	}

	for(int i=0; i<DOUT; i++)
		hash[i] = (float*) malloc( sizeof(float) * DIN);

	middle_result_dfe = (int16_t**) malloc( sizeof(int16_t*) * REDUNDANT);
	middle_result_cpu = (int16_t**) malloc( sizeof(int16_t*) * REDUNDANT);
	for(int i=0; i<REDUNDANT; i++){
		middle_result_dfe[i] = (int16_t*) malloc(sizeof(int16_t) * input_size * 8);
		middle_result_cpu[i] = (int16_t*) malloc(sizeof(int16_t) * input_size * 8);
	}
}

void generate_data(){
	printf("generate data\n");
	srand(0);
	// generate input
	int length = input_size * DIN;
	for(int i=0; i<length; i++)
		input[i] = (float)(rand()) / (float)(RAND_MAX) * max_val;

	// generate hash function and formalization
	for(int i=0; i<DOUT; i++)
		for(int j=0; j<DIN; j++)
			hash[i][j] = (float)(rand()) / (float)(RAND_MAX);
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
	int output_length = input_size * 2;
	for(int red=0; red<REDUNDANT; red++){
		for(int j=0; j<output_length; j++){
			output_cpu[red][j] = 0;
			output_dfe[red][j] = 0;
		}
	}
}

void release_memory(){
	printf("release memory\n");
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
	
	for(int i=0; i<REDUNDANT; i++){
		free(middle_result_cpu[i]);
		free(middle_result_dfe[i]);
	}
	free(middle_result_cpu);
	free(middle_result_dfe);
}

void lsh_cpu(){
	printf("lsh cpu\n");
	int* temp = (int*) malloc(sizeof(int) * DOUT);
	float* mult = (float*) malloc(sizeof(float) * DOUT);
	for(int i=0; i<input_size; i++){
		// make projection
		for(int j=0; j<DOUT; j++){
			float data = 0.0f;
			for(int k=0; k<DIN; k++)
				data += input[i * DIN + k] * hash[j][k];
			mult[j] = data;
		}
		// calculate index in each dimension
		for(int red=0; red<REDUNDANT; red++){
			for(int j=0; j<DOUT; j++){
				temp[j] = (int)(( mult[j] - center[red][j]) / cell_width);
			}
			// make the final hash
			int64_t first = 0;
			int64_t second = 0;
			for(int j=0; j<DOUT/2-1; j++){
				temp[j] = temp[j] & 0x0000ffff;
				first += temp[j];
				first = first << 16;
			}
			temp[DOUT/2-1] = temp[DOUT/2-1] & 0x0000ffff;
			first += temp[DOUT/2-1];

			for(int j=DOUT/2; j<DOUT-1; j++){
				temp[j] = temp[j] & 0x0000ffff;
				second += temp[j];
				second = second << 16;
			}
			temp[DOUT-1] = temp[DOUT-1] & 0x0000ffff;
			second += temp[DOUT-1];
			output_cpu[red][2 * i] = first;
			output_cpu[red][2 * i + 1] = second;

			for(int j=0; j<8; j++)
				middle_result_cpu[red][i * 8 + j] = temp[j];
		}
	}
	free(temp);
	free(mult);
}

void set_mapped_rom(LSH_actions_t* actions){
	double** hashFunction = (double**) &(actions->inmem_LSHKernel_hashFunction0000);
	for(int i=0; i<DOUT; i++){
		hashFunction[i] = (double*)malloc(sizeof(double) * DIN);
		for(int j=0; j<DIN; j++)
			hashFunction[i][j] = hash[i][j];
	}
	double** centerPoint = (double**) &(actions->inmem_LSHKernel_centerPoint0000);
	for(int i=0; i<DOUT; i++){
		centerPoint[i] = (double*)malloc(sizeof(double) * REDUNDANT);
		for(int j=0; j<REDUNDANT; j++)
			centerPoint[i][j] = center[j][i];
	}
}

void lsh_dfe(){
	printf("lsh dfe\n");
	
	max_file_t* mf = LSH_init();
	max_engine_t* me = max_load(mf, "*");

	LSH_actions_t actions;
	actions.param_N = input_size;
	actions.param_cellWidth = cell_width;
	actions.instream_input_cpu = input;

	actions.outstream_output_cpu0 = middle_result_dfe[0];
	actions.outstream_output_cpu1 = middle_result_dfe[1];
	actions.outstream_output_cpu2 = middle_result_dfe[2];
	actions.outstream_output_cpu3 = middle_result_dfe[3];
	
	set_mapped_rom(&actions);

	LSH_run(me, &actions);

	max_unload(me);
}

void check_output(){
	printf("check output\n");
	
	int output_length = input_size * 8;
	int counter = 0;
	for(int red=0; red<4; red++)
		for(int i=0; i<output_length; i++)
			if(middle_result_cpu[red][i] == middle_result_dfe[red][i])
				counter++;
	printf("%d same over %d\n", counter, output_length);
	if(counter != output_length){
		printf("Error, check files\n");
		to_file();
	}
	

	/*
	int output_length = input_size * 2;
	int counter = 0;
	for(int red=0; red<REDUNDANT; red++){
		for(int i=0; i<output_length; i++)
			if(output_cpu[red][i] == output_dfe[red][i])
				counter++;
	}

	printf("%d same over %d\n", counter, output_length * REDUNDANT);
	if(counter != output_length){
		printf("Error!!! please check files\n");
		to_file();
	}
	*/
}

void to_file(){
	printf("to file\n");
	int output_length = input_size * 8;
	//int output_length = input_size * DOUT;
    FILE* fp1 = fopen("answer_cpu", "w");
	
	for(int red=0; red<REDUNDANT; red++){
		for(int i=0; i<output_length; i++)
			fprintf(fp1, "%d\n", middle_result_cpu[red][i]);
		fprintf(fp1, "\n");
	}
	
	/*
	for(int i=0; i<output_length; i++)
		fprintf(fp1, "%f\n", middle_result_cpu[i]);
	*/
    fclose(fp1);
				
    FILE* fp2 = fopen("answer_dfe", "w");
	for(int red=0; red<REDUNDANT; red++){
		for(int i=0; i<output_length; i++)
			fprintf(fp2, "%d\n", middle_result_dfe[red][i]);
		fprintf(fp2, "\n");
	}
	
	/*
	for(int i=0; i<output_length; i++)
		fprintf(fp2, "%f\n", middle_result_dfe[i]);
	*/
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

