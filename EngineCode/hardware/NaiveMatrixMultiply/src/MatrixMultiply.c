
#include <stdlib.h>
#include <string.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

const int num_points_cell = 10;
const int num_neighbour = 25;
const int num_rows = 100;
const int num_cells = 2000;

float* answer_cpu;
float* answer_dfe;

static inline void* safe_malloc(size_t size){
	void* output = malloc(size);
	if(!output){
		fprintf(stderr, "Error, can not alloc memory of size %lu\n", size);
		exit(1);
	}
	return output;
}

void calc_cell(float* arr, int which, int current, float* accumulator){
	int num_of_points = num_cells * num_points_cell;
	int length = num_of_points * 2;

	int cell_offset = 0;
	if(which >= 0 && which <= 4)
		cell_offset = -2 * num_rows + which - 2;
	else if(which >=5 && which <= 9)
		cell_offset = -1 * num_rows + which - 7;
	else if(which >= 10 && which <= 14)
		cell_offset = which - 12;
	else if(which >= 15 && which <= 19)
		cell_offset = 1 * num_rows + which - 17;
	else if(which >= 20 && which <= 24)
		cell_offset = 2 * num_rows + which - 22;
	cell_offset = cell_offset * num_points_cell * 2;

	int begin = current + cell_offset;
	int end = begin + 2 * num_points_cell;
	if(begin < 0 || end > length)
		return;

	for(int i=begin; i<end; i++)
		*accumulator += arr[i];
}

void MatrixMultiply_cpu(float eps, int minPts, int num_of_points, float* arr, float* answer){
	int length = num_of_points * 2;
	for(int i=0; i<length; i++){
		float accumulator = 0;
		for(int which=0; which<num_neighbour; which++)
			calc_cell(arr, which, i, &accumulator);
		answer[i] = accumulator;
	}
}

void test_cpu(){
	int num_of_points = num_cells * num_points_cell;
	int length = num_of_points * 2;
	float* arr = (float*)safe_malloc(sizeof(float) * length);
	for(int i=0; i<length; i++)
		arr[i] = 1.0;

	float* answer_cpu = (float*)safe_malloc(sizeof(float) * length);
	float eps = 0.1;
	int minPts = 5;
	MatrixMultiply_cpu(eps, minPts, num_of_points, arr, answer_cpu);
}

void test_dfe(){
	int num_of_points = num_cells * num_points_cell;
	int length = num_of_points * 2;
	float* arr = (float*)safe_malloc(sizeof(float) * length);
	for(int i=0; i<length; i++)
		arr[i] = 1.0;

	answer_dfe = (float*)safe_malloc(sizeof(float) * length);
	float eps = 0.1;
	int minPts = 5;
	MatrixMultiply(eps, minPts, num_of_points, arr, answer_dfe);

}

void check_result(){
	int num_of_points = num_cells * num_points_cell;
	int length = num_of_points * 2;
	int cnt_correct = 0;
	for(int i=0; i<length; i++)
		if(answer_cpu[i] == answer_dfe[i])
			cnt_correct++;
	printf("%d correct over %d\n", cnt_correct, length);

	int begin = 5 * num_rows * num_points_cell * 2;
	int end = length - begin;
	cnt_correct = 0;
	for(int i=begin; i<end; i++)
		if(answer_cpu[i] == answer_dfe[i])
			cnt_correct++;
	printf("%d correct over %d\n", cnt_correct, end - begin);

}

void to_file(){
	int length = num_cells * num_points_cell * 2;
	FILE* fp1 = fopen("answer_cpu", "w");
	for(int i=0; i<length; i++)
		fprintf(fp1, "%f\n", answer_cpu[i]);
	fclose(fp1);

	FILE* fp2 = fopen("answer_dfe", "w");
	for(int i=0; i<length; i++)
		fprintf(fp2, "%f\n", answer_dfe[i]);
	fclose(fp2);
}

int main(void)
{
    printf("#########################################################################################\n");
    printf("Programming start\n");
    
	printf("Running cpu\n");
	test_cpu();
    printf("Running dfe\n");
	test_dfe();
    
	printf("Checking\n");
	check_result();
    printf("#########################################################################################\n");
    return 0;
}

