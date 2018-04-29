#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int main (int argc, char **argv) {

	//seed value for the randomizer 
  double seed = clock(); //this will make your program run differently everytime
  //double seed = 0; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  int bufferSize = 1024;
  unsigned char *message = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

  printf("Enter a message to encrypt: ");
  int stat = scanf (" %[^\n]%*c", message); //reads in a full line from terminal, including spaces

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h;

  printf("Reading file.\n");

  /* Q2 Complete this function. Read in the public key data from public_key.txt,
    convert the string to elements of Z_p, encrypt them, and write the cyphertexts to 
    message.txt */
  FILE *file = fopen("public_key.txt", "r");
  fscanf(file, "%u\n%u\n%u\n%u", &n,&p,&g,&h);
 
  
  unsigned int cpi = n/8;


  unsigned int Numchrs = mystrlen(message);  
  unsigned int Nints = (mystrlen(message)/cpi);

  
  unsigned int *ary1 = (unsigned int *)malloc(Nints*sizeof(unsigned int));
  
  padString(message, cpi);
  convertStringToZ(message, Numchrs, ary1, Nints);

  unsigned int *ary2 = (unsigned int *)malloc(Nints*sizeof(unsigned int));
  

  ElGamalEncrypt(ary1, ary2, Nints, p, g, h);

  printf("Writing to file.\n");
  
  FILE *newfile = fopen("message.txt", "w");
  fprintf(newfile, "%u\n", Nints);
  

  for(int x = 0; x<Nints; x++){

    fprintf(newfile, "%u %u\n", ary1[x], ary2[x]);
  }
  
  fclose(file);
  fclose(newfile);
  return 0;
}
