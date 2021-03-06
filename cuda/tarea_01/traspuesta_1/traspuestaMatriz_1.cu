//
//  traspuestaMatriz.cu
//  
//
//  Created by Amilcar Meneses Viveros on 15/02/18.
//
//

#include <stdio.h>

#define M 10
#define N 10

double a[M][N], b[M][N], c[M][N];

__global__ void kernelIncrementaMatriz(double *b, double *c, int m, int n) {
       int i = threadIdx.x+blockIdx.x*blockDim.x;
       int j = threadIdx.y+blockIdx.y*blockDim.y;

       while (i<m) {
         j = threadIdx.y+blockIdx.y*blockDim.y;
         while (j<n) {
             b[i*n+j]+=c[i*n+j];
             j += blockDim.x*gridDim.x; 
         }
         i+=blockDim.y*gridDim.y;
      }
}

void traspuestaMatrizOnDevice(double a[][N], double b[][N], double c[][N], int m, int n) {
    int size=m*n*sizeof(double);
    double *bD, *cD;
    dim3 nb(2,2);
    dim3 nt(5,5);
   
    cudaSetDevice(0);  
    // 1. Reservar memoria
    cudaMalloc(&bD, size);  cudaMalloc(&cD, size);

    // 2. Subir datos del Host a Device
    cudaMemcpy(bD, b, size, cudaMemcpyDefault);
    cudaMemcpy(cD, c, size, cudaMemcpyDefault);

    // 3. Ejecutar kernel
    kernelIncrementaMatriz<<<nb, nt>>>(bD, cD, m, n);
    
    // 4. Bajar datos del Device al Host
    cudaMemcpy(a, bD, size, cudaMemcpyDefault);
 
   // 5.Libera memoria
    cudaFree(bD); cudaFree(cD);
}

int main() {
    int i, j;
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
            b[i][j] = c[i][j] = i+j;
        }
    }
    traspuestaMatrizOnDevice(a, b, c, M, N);

    for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
            printf("%3.2lf ", a[i][j]);
        }
        printf("\n");
    }
}

