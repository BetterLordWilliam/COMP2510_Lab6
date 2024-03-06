// A01372608

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Function Definitions
long long* innitFiles(int *bitNum, int *ncount);
void getMax(long long *mxN, int *bN);
void getMin(long long *mnN, int *bN);
void write(long long *arr, int *bN, long long *mxN, long long *mnN, int *c);

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
  unsigned long long maxN = 0, *mxN;
  unsigned long long minN = 0, *mnN; 

  cP = &count, bP = &bitNum; 
  mxN = &maxN, mnN = &minN;

  // Read file, get bits and numbers
  long long *numbers = innitFiles(bP, cP);

  // Calculate max and min
  getMax(mxN, bP);
  getMin(mnN, bP);

  // Test stuff
  printf("%lld %d\n", *bP, *cP);
  for (int i = 0; i != *cP; i++) {
    printf("%lld ", numbers[i]);
  }
  printf("\n");

  // Determine the output
  write(numbers, bP, mxN, mnN, cP);

  return 0;
}

/**
 * innitFiles:      reads the file input, assings bitNum and ncount as the number of bits and number of numbers.
 * param *bitNum:   the number of bits 
 * param *ncount:   the number of numbers
 * returns:         *arr to the numbers
*/
long long* innitFiles(int *bitNum, int *ncount) {
  char check = 0;

  // Determine the number of numbers
  while ((check = fgetc(in)) != 'E') {
    if (check == '\n') {
      *ncount = (*ncount + 1);
    }
  }
  *ncount = (*ncount - 1);

  // Create array that stores the particles
  long long *arr = malloc(*ncount * sizeof(long long));

  // set buffer back to the beginning
  char buff[100];
  fseek(in, 0, SEEK_SET);
  fgets(buff, 100, in);
  sscanf(buff, "%d", bitNum);
  int sCount = 0;

  // Get the numbers
  while (fgets(buff, 100, in) != NULL) {
    long long tempN = 0;
    if (sscanf(buff, "%lld", &tempN) != EOF) {
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
void getMax(long long *mxN, int *bN) {
  long long t = 1;
  int bNC = *bN - 1;
  while (bNC != 0) {
    t = t << 1;   // basically multiply by 2
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
void getMin(long long *mnN, int *bN) {
  long long t = 1;
  int bNC = *bN - 1;
  while (bNC != 0) {
    t = t << 1;   // basically multiply by 2
    bNC--;
  }
  t*=-1;
  printf("%lld\n", t);
  *mnN = t;
}

long long abs_val(long long x) {
    long long mask = x >> (sizeof(long long) * 8 - 1);
    return (x ^ mask) - mask;
}

void write(long long *arr, int *bN, long long *mxN, long long *mnN, int *c) {
  // Print bounds and hex values if there are no numbers
  // Not important to the calculation
  if (*c == 0) {
    fprintf(out, "min: %lld\t0x%016llx\nmax: %lld\t0x%016llx\n", *mnN, *mnN, *mxN, *mxN);
    return;
  } 

  // Long long data size so that most bits are avaliable.
  long long prod = arr[0] * arr[1];
  
  // Get sign bits
  int sign1 = ((arr[0] >> (sizeof(int) * 8)-1) & 1);
  int sign2 = ((arr[1] >> (sizeof(int) * 8)-1) & 1);

  // sign of number 1
  printf("sign0:%u\n", sign1);
  printf("sign1:%u\n", sign2);

  printf("prodNAT:%lld\n", prod);

  // Positive result, overflow risk
  if (sign1 == sign2) {
    long long diff = prod - *mxN;
    long long is_a_greater = ~(diff >> (sizeof(long long) * 8 - 1)) & 1;

    printf("Is a greater? %lld\n", is_a_greater);

    prod = (is_a_greater) ? *mxN : prod;

  // Negative result, underflow risk
  } else {
    long long a = abs_val(prod);
    long long b = abs_val(*mnN);

    long long diff = a - b;
    long long is_a_smaller = diff >> (sizeof(long long) * 8 - 1) & 1;

    printf("Is a (abs) smaller? %lld\n", is_a_smaller);

    prod = (is_a_smaller) ? prod : *mnN;

  } 

  printf("prod %lld\n", prod);
}