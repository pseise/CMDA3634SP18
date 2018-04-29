#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"




  __device__ unsigned int CUDAModProd(unsigned int a, unsigned int b, unsigned int p){
    unsigned int za = a;
    unsigned int ab = 0;
  
    while (b > 0) {
      if (b%2 == 1) ab = (ab +  za) % p;
      za = (2 * za) % p;
      b /= 2;
    }
    return ab;
  }

  __device__ unsigned int CUDAModExp(unsigned int a, unsigned int b, unsigned int p){

    unsigned int z = a;
    unsigned int aExpb = 1;
  
    while (b > 0) {
      if (b%2 == 1) aExpb = CUDAModProd(aExpb, z, p);
      z = CUDAModProd(z, z, p);
      b /= 2;
    }
    return aExpb;

  }


  __global__  void zaxbys(int p,int g, int h, unsigned int *secretKey)
  {
      for (unsigned int n = blockIdx.x * blockDim.x + threadIdx.x; n <p-1; n = n + blockDim.x * gridDim.x) {

        if(CUDAModExp(g, n+1, p) == h){

          printf("Secret Key found! x = %u \n", n+1);
          
            secretKey[0] = n+1;

        }
  }
  }
     

  //declare storage for an ElGamal cryptosytem
  
  
int main (int argc, char **argv) {
   unsigned int n, p, g, h, x;
   unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  
    FILE *filly = fopen("public_key.txt", "r");
    fscanf(filly, "%u\n%u\n%u\n%u", &n,&p,&g,&h);

    FILE *messy = fopen("message.txt", "r");
    fscanf(messy, "%u\n", &Nints);
    
   
    
    unsigned int cpi = n/8;
    
    unsigned int *ary1 = (unsigned int *)malloc(Nints*sizeof(unsigned int));
    
    unsigned int *ary2 = (unsigned int *)malloc(Nints*sizeof(unsigned int));

    
    
    
    

  for(int x = 0; x<Nints; x++){

    fscanf(messy, "%u %u\n", &ary1[x], &ary2[x]);
  }

  
  // find the secret key
  /* Q4 Make the search for the secret key parallel on the GPU using CUDA. */
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    int Nthreads = 32;

    int Nblocks = (p+Nthreads-1)/Nthreads;

    unsigned int *h_x = (unsigned int *) malloc(Nints*sizeof(unsigned int));

    unsigned int *d_a;

    cudaMalloc(&d_a, sizeof(unsigned int));
    double startTime = clock();
    zaxbys <<<Nblocks,Nthreads>>> (p,g,h,d_a);
    cudaDeviceSynchronize();
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;



    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
    
    cudaMemcpy(h_x, d_a, sizeof(unsigned int), cudaMemcpyDeviceToHost);

    x = *h_x;
  }

  
  ElGamalDecrypt(ary1, ary2, Nints, p, x);

  unsigned int Numchrs = cpi * Nints;

  unsigned char *origins = (unsigned char *) malloc(Numchrs*sizeof(unsigned char));

  convertZToString(ary1, Nints, origins, Numchrs);

  printf("The message is %s\n", origins);

  fclose(messy);
  fclose(filly);
  



  



  return 0;
}
