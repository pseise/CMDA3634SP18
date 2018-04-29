#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"


int main (int argc, char **argv) {

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); 
  fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
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
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }

  /* Q3 After finding the secret key, decrypt the message */
  ElGamalDecrypt(ary1, ary2, Nints, p, x);

  unsigned int Numchrs = cpi * Nints;

  unsigned char *origins = (unsigned char *) malloc(Numchrs*sizeof(unsigned char));

  convertZToString(ary1, Nints, origins, Numchrs);

  printf("The message is %s\n", origins);

  fclose(messy);
  fclose(filly);
  return 0;
}
