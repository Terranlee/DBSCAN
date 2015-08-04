
#include <stdlib.h>
#include <string.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

uint32_t** x;
uint32_t** y;
int size = 100;

uint32_t* input_data;
uint32_t* output_cpu;
uint32_t* output_dfe;

int input_length = size * size * 2;

void to_file();

void prepare_data(){
	x = (uint32_t**)malloc(sizeof(uint32_t*) * size);
	for(int i=0; i<size; i++)
		x[i] = (uint32_t*)malloc(sizeof(uint32_t) * size);
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			x[i][j] = i + j;
	
	y = (uint32_t**)malloc(sizeof(uint32_t*) * size);
	for(int i=0; i<size; i++)
		y[i] = (uint32_t*)malloc(sizeof(uint32_t) * size);
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			y[i][j] = 3 * i - 5 * j;

	input_data = (uint32_t*)malloc(sizeof(uint32_t) * input_length);
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++){
			input_data[i * size + j] = x[i][j];
			input_data[i * size + j + 1] = y[i][j];
		}

	int output_length = input_length / 2;
	output_cpu = (uint32_t*)malloc(sizeof(uint32_t) * output_length);
	output_dfe = (uint32_t*)malloc(sizeof(uint32_t) * output_length);
	for(int i=0; i<output_length; i++)
		output_cpu[i] = output_dfe[i] = 0;
}

void test_cpu(){
	for(int i=0; i<input_length; i++){
		if(input_data[i] == invalid)
			output_cpu[i] = 1234;
		else{
			uint32_t var = input_data[i] * input_data[i] * input_data[i];
			var = var - 3 * input_data[i];
			output_cpu[i] = var;
		}
	}
}

void test_dfe(){
	printf("running DFE\n");
	MatrixMultiply(input_length, invalid, input_data, output_dfe);
	printf("running DFE end\n");
}

void check_output(){
	int output_length = input_length;
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
	int output_length = input_length;
    FILE* fp1 = fopen("answer_cpu", "w");
    for(int i=0; i<output_length; i++)
        fprintf(fp1, "%d\n", output_cpu[i]);
    fclose(fp1);
				
    FILE* fp2 = fopen("answer_dfe", "w");
    for(int i=0; i<output_length; i++)
        fprintf(fp2, "%d\n", output_dfe[i]);
    fclose(fp2);
}

void release(){
	for(int i=0; i<size; i++)
		free(matrix[i]);
	free(matrix);

	free(input_data);
	free(output_cpu);
	free(output_dfe);
}

int main(void)
{
    printf("#########################################################################################\n");
    printf("Programming start\n");
    
	prepare_data();
	test_cpu();
	test_dfe();
	check_output();
	release();
	
	printf("#########################################################################################\n");
    return 0;
}

