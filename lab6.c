#include <stdio.h>
#include <stdlib.h>

// Function Definitions
int* innitFiles(int *bitNum, int *ncount);
void getMax(int *mxN, int *bN);
void getMin(int *mnN, int *bN);

FILE *in;
FILE *out;

/**
 * main:            Drives the program
 * param argc:      Argument count 
 * param *argv[]:   Arguments themselves 
*/
int main(int argc, char *argv[]) {
  // Lab setup
  if (argc != 3)
    return 1;
  
  in = fopen(argv[1], "r");
  out = fopen(argv[2], "w");

  int count = 0, *cP;
  int bitNum = 0, *bP;
  int maxN = 0, *mxN;
  int minN = 0, *mnN; 

  cP = &count, bP = &bitNum; 
  mxN = &maxN, mnN = &minN;

  // Read file, get bits and numbers
  int *numbers = innitFiles(bP, cP);

  // Calculate max and min
  getMax(mxN, bP);
  getMin(mnN, bP);

  // Test stuff
  printf("%d %d\n", *bP, *cP);
  for (int i = 0; i < *cP; i++) {
    printf("%d ", numbers[i]);
  }
  printf("\n");

  return 0;
}

/**
 * innitFiles:      reads the file input, assings bitNum and ncount as the number of bits and number of numbers.
 * param *bitNum:   the number of bits 
 * param *ncount:   the number of numbers
 * returns:         *arr to the numbers
*/
int* innitFiles(int *bitNum, int *ncount) {
  char check = 0;

  // Determine the number of numbers
  while ((check = fgetc(in)) != 'E') {
    if (check == '\n') {
      *ncount = (*ncount + 1);
    }
  }
  *ncount = (*ncount - 1);

  // Create array that stores the particles
  int *arr = malloc(*ncount * sizeof(int));

  // set buffer back to the beginning
  char buff[10];
  fseek(in, 0, SEEK_SET);
  fgets(buff, 10, in);
  sscanf(buff, "%d", bitNum);
  int sCount = 0;

  // Get the numbers
  while (fgets(buff, 10, in) != NULL) {
    int tempN = 0;
    if (sscanf(buff, "%d", &tempN) != EOF) {
      arr[sCount++] = tempN;
    }
  }

  return arr;
}

/**
 * getMax:      Returns the maxNumber for specified bitwidth.
 * param *mnN:  The address where this max is to be stored.
 * param *bN:   The number of bits.
 * 
 * formula max = 2^(n-1) - 1, signed
*/
void getMax(int *mxN, int *bN) {
  long long t = 1;
  int bNC = *bN - 1;
  while (bNC != 0) {
    t *= 2;
    bNC--;
  }
  t--;
  printf("%lld\n", t);
  *mxN = t;
}

/**
 * getMin:      Returns the minNumber for specified bitwidth.
 * param *mnN:  The address where this min is to be stored.
 * param *bN:   The number of bits.
 * 
 * formula min = -2^(n-1), signed
*/
void getMin(int *mnN, int *bN) {
  long long t = 1;
  int bNC = *bN - 1;
  while (bNC != 0) {
    t *= 2;
    bNC--;
  }
  t*=-1;
  printf("%lld\n", t);
  *mnN = t;
}