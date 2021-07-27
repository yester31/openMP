// 2021/07/27 BY YHPARK

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(int argc, char*argv[])
{
	int thread_count = strtol(argv[1], NULL, 10);

	#pragma omp parallel num_threads(thread_count)
	printf("[Thread %d] Hello OpenMP!\n", omp_get_thread_num());

}

