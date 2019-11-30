#include <stdio.h>
#include  <math.h>

// sin(x*x)*cos(x)*x -  x*x  

#define XMIN  0.0  
#define XMAX  10.0 
#define  N 10000 

double vh[N], vd[N]; 

__device__  __host__ double funcion(double x) {
   return sin(x*x)*cos(x)*x -  x*x; 
}

__global__ void kernelEvaluaFuncion(double *v, double xmin, double xmax, int n){
  int i=threadIdx.x + blockIdx.x*blockDim.x;  
  double dx= (xmax-xmin)/(double)(n-1);
  double x = xmin + i*dx;  
  v[i] = funcion(x); 
}


void llenaPolinomioEnDevice(double *v, int n) {
    double *vD; 
    int size = n*sizeof(double); 

    cudaMalloc(&vD, size); 
    kernelEvaluaFuncion<<<10, 1000>>>(vD, XMIN, XMAX, n); 
    cudaMemcpy(v, vD, size, cudaMemcpyDeviceToHost); 
    cudaFree(vD); 
}

void llenaPolinomioEnHost(double *v, int n) {
   int i; 
   double x, dx;
   dx = (XMAX-XMIN)/(double)(N-1);

   x = XMIN; 
   for (i=0; i<N; i++) {
      v[i] = funcion(x); 
      x+=dx;
   }
}

int main() {
   int i; 
   double x, dx; 
   dx = (XMAX-XMIN)/(double)(N-1); 

   llenaPolinomioEnHost(vh, N); 
   llenaPolinomioEnDevice(vd, N); 

   x = XMIN; 
   for (i=0; i<N; i++) {
      printf("%4d %4.2lf    %6.12lf    %6.12lf\n", i, x, vh[i], vd[i]); 
      x+=dx; 
   }
}

