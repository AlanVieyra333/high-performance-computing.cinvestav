//
//  mulMat.cu
//  
//
//  Created by Amilcar Meneses Viveros on 15/02/18.
//
//

#include <stdio.h>

#define M 1000
#define N 1000 

// double a[M][N], b[M][N], c[M][N];
double *a, *b, *c; 
float elapsedtime_total;
float elapsedtime_kernel; 

__global__ void kernelmultiplicaMatrices(double *a, double *b, double *c, int m, int n) {
    int i = threadIdx.x + blockDim.x*blockIdx.x;
    int j = threadIdx.y + blockDim.y*blockIdx.y;
    int k; 
    double tmp;    
 
    tmp = 0; 
    for (k=0; k<n; k++) {
        tmp += b[i*m+k]*c[k*m+j]; 
    }
   a[i*m+j] = tmp;     
}

void multiplicaMatricesEnDevice(double *a, double *b, double *c, int m, int n) {
    int size=m*n*sizeof(double);
    double *aD, *bD, *cD;
    dim3 nb(50,50);
    dim3 nt(20,20);
    cudaEvent_t start, stop; 
    cudaEvent_t startk, stopk;

    cudaSetDevice(0);  
    cudaEventCreate(&start); 
    cudaEventCreate(&stop); 
    cudaEventCreate(&startk);
    cudaEventCreate(&stopk);

    cudaEventRecord(start, 0); 

    // 1. Reservar memoria
    cudaMalloc(&aD, size); cudaMalloc(&bD, size);  cudaMalloc(&cD, size);

    // 2. Subir datos del Host a Device
    cudaMemcpy(bD, b, size, cudaMemcpyHostToDevice);
    cudaMemcpy(cD, c, size, cudaMemcpyHostToDevice);

    // 3. Ejecutar kernel
    cudaEventRecord(startk, 0);
       kernelmultiplicaMatrices<<<nb, nt>>>(aD, bD, cD, m, n);
    cudaEventRecord(stopk, 0); 
    cudaEventSynchronize(stopk);

    // 4. Bajar datos del Device al Host
    cudaMemcpy(a, aD, size, cudaMemcpyDeviceToHost);
 
   // 5.Libera memoria
    cudaFree(aD); cudaFree(bD); cudaFree(cD);

   cudaEventRecord(stop, 0); 
   cudaEventSynchronize(stop); 

   cudaEventElapsedTime(&elapsedtime_total, start, stop); 
   cudaEventElapsedTime(&elapsedtime_kernel, startk, stopk);

   cudaEventDestroy(start); cudaEventDestroy(stop); 
   cudaEventDestroy(startk); cudaEventDestroy(stopk);

}

int main() {
    int i, j;

    a = (double*)malloc(M*N*sizeof(double));
    b = (double*)malloc(M*N*sizeof(double)); 
    c = (double*)calloc(M*N, sizeof(double)); 

    for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
            b[i*N+j] = i+j;
        }
        c[i*N+i] = 2.0;

    }

    multiplicaMatricesEnDevice(a, b, c, M, N);
    for (i=M-1; i<M; i++) {
        for (j=0; j<N; j++) {
            printf("%3.2lf ", a[i*N+j]);
        }
        printf("\n");
    } 
    free(a); free(b); free(c); 

    printf("Tiempo total cuda %4.6fms\n", elapsedtime_total);
    printf("Tiempo kernel cuda %4.6fms\n", elapsedtime_kernel);  
    return 0; 
}

