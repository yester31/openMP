#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void transpose(float *A, float *B, int n) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			B[j*n + i] = A[i*n + j];
		}
	}
}

void gemm(float *A, float *B, float *C, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			float dot = 0;
			for (k = 0; k < n; k++) {
				dot += A[i*n + k] * B[k*n + j];
			}
			C[i*n + j] = dot;
		}
	}
}

void gemm_omp(float *A, float *B, float *C, int n)
{
	#pragma omp parallel
	{
		int i, j, k;
		#pragma omp for
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				float dot = 0;
				for (k = 0; k < n; k++) {
					dot += A[i*n + k] * B[k*n + j];
				}
				C[i*n + j] = dot;
			}
		}

	}
}

void gemmT(float *A, float *B, float *C, int n)
{
	int i, j, k;
	float *B2;
	B2 = (float*)malloc(sizeof(float)*n*n);
	transpose(B, B2, n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			float dot = 0;
			for (k = 0; k < n; k++) {
				dot += A[i*n + k] * B2[j*n + k];
			}
			C[i*n + j] = dot;
		}
	}
	free(B2);
}

void gemmT_omp(float *A, float *B, float *C, int n)
{
	float *B2;
	B2 = (float*)malloc(sizeof(float)*n*n);
	transpose(B, B2, n);
	#pragma omp parallel
	{
		int i, j, k;
		#pragma omp for
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				float dot = 0;
				for (k = 0; k < n; k++) {
					dot += A[i*n + k] * B2[j*n + k];
				}
				C[i*n + j] = dot;
			}
		}

	}
	free(B2);
}

int main() {
	int i, n;
	float *A, *B, *C, dtime;

	n = 1024;
	A = (float*)malloc(sizeof(float)*n*n);
	B = (float*)malloc(sizeof(float)*n*n);
	C = (float*)malloc(sizeof(float)*n*n);
	for (i = 0; i < n*n; i++) { A[i] = rand() / RAND_MAX; B[i] = rand() / RAND_MAX; }

	dtime = omp_get_wtime();
	gemm(A, B, C, n);
	dtime = omp_get_wtime() - dtime;
	printf("no transpose  no openmp :: %f\n", dtime);

	dtime = omp_get_wtime();
	gemm_omp(A, B, C, n);
	dtime = omp_get_wtime() - dtime;
	printf("no transpose, openmp :: %f\n", dtime);

	dtime = omp_get_wtime();
	gemmT(A, B, C, n);
	dtime = omp_get_wtime() - dtime;
	printf("tranpose, no openmp :: %f\n", dtime);

	dtime = omp_get_wtime();
	gemmT_omp(A, B, C, n);
	dtime = omp_get_wtime() - dtime;
	printf("transpose, openmp :: %f\n", dtime);

	free(A);
	free(B);
	free(C);

	return 0;

}