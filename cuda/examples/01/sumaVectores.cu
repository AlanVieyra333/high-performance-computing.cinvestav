

#include <stdio.h>

#define N 10000 

float a[N], b[N], c[N]; 

__global__ void kernelSumaVectores(float *a, float *b, float *c, int n)  
{
    int i = threadIdx.x+blockIdx.x*blockDim.x;

    while (i<n) {
        c[i]=a[i]+b[i];
        i+= blockDim.x*gridDim.x; 
   }
}

void sumaVectoresEnDevice(float *a, float *b, float *c, int n) {
    float *aD, *bD, *cD; 
    int size=n*sizeof(float); 

    cudaSetDevice(0); 

    // 1. Asignar memoria 
    cudaMalloc(&aD, size); 
    cudaMalloc(&bD, size); 
    cudaMalloc(&cD, size); 
    
   // 2. Copiar datos del Host al Device 
   cudaMemcpy(aD, a, size, cudaMemcpyHostToDevice);    
   cudaMemcpy(bD, b, size, cudaMemcpyHostToDevice);    

   // 3. Ejecutar kernel 
   kernelSumaVectores<<<10, 1024>>>(aD, bD, cD, n);

   // 4. Copiar datos del device al Host
   cudaMemcpy(c, cD, size, cudaMemcpyDeviceToHost);    

   // 5. Liberar Memoria
   cudaFree(aD); cudaFree(bD); cudaFree(cD); 
}

int main() {
   int i; 

  for (i=0; i<N; i++) { 
      a[i] = b[i] = i; 
  }

  sumaVectoresEnDevice(a, b, c, N); 

  for (i=0; i<N; i++) { 
      printf("c[%d] = %f\n", i, c[i]); 
  }

}


