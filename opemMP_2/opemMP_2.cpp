// 2021/07/28 BY YHPARK

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#define VECTOR_SIZE 102

using namespace std;

void init_vector_data(int * arr, int vector_size, int start = 0) {
	#pragma omp parallel
	#pragma omp for
	for (int idx = 0; idx < vector_size; idx++) {
		arr[idx] = start + idx;
	}
}

void vector_sum(int * output, int * input_1, int * input_2, int vector_size, int num_thread) {
	int numElePerThread = vector_size / num_thread;
	int *start = new int[num_thread];
	int *end = new int[num_thread];

	printf("Work decomposition \n");
	for (int tID = 0; tID < num_thread; tID++) {
		start[tID] = numElePerThread * tID;
		end[tID] = numElePerThread * (tID + 1);

		if (tID == num_thread - 1) //for the last thread
			end[num_thread - 1] = vector_size;

		printf("[T%d] %d ~ %d\n", tID, start[tID], end[tID]);
	}

	#pragma omp parallel num_threads(num_thread)
	{ // parallel region 
		int tID = omp_get_thread_num();
		printf("tID : %d \n", tID);
		for (int i = start[tID]; i < end[tID]; i++)
			output[i] = input_1[i] + input_2[i];
	}
}

void vector_sum_loop_construct(int * output, int * input_1, int * input_2, int vector_size, int num_thread) 
{
	#pragma omp parallel num_threads(num_thread)
	{ // parallel region 
		int tID = omp_get_thread_num();
		printf("tID : %d \n", tID);	
		#pragma omp for
		for (int i = 0; i < vector_size; i++) {
			output[i] = input_1[i] + input_2[i];
		}
	}
}

int main() {

	int arr_a[VECTOR_SIZE];
	int arr_b[VECTOR_SIZE];
	int arr_c[VECTOR_SIZE];
	init_vector_data(arr_a, VECTOR_SIZE);
	init_vector_data(arr_b, VECTOR_SIZE, 2);

	int numThreads = 16;
	//vector_sum(arr_c, arr_a, arr_b, VECTOR_SIZE, numThreads);
	vector_sum_loop_construct(arr_c, arr_a, arr_b, VECTOR_SIZE, numThreads);

	for (int i = 0; i < VECTOR_SIZE; i++) {
		printf("%d + %d = %d\n", arr_a[i], arr_b[i], arr_c[i]);
	}

	return 0;
}