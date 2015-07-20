
#include <stdlib.h>
#include <string.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

const int m_num_points_cell = 10;

void test_dfe(){
	int num_cells = 1000;
	int length = num_cells * m_num_points_cell * 2;
	float* arr = malloc(sizeof(float) * length);
	for(int i=0; i<length; i++)
		arr[i] = 1.0;

	float answer = malloc(sizeof(float) * num_cells);
	float eps = 0.1;
	MatrixMultiply(num_cells, eps, arr, answer);

}

int main(void)
{
    printf("#########################################################################################\n");
    printf("Programming start\n");
    
    printf("Running dfe\n");
	test_dfe();
    

    printf("Checking pass!!\n");
    printf("#########################################################################################\n");
    return 0;
}
