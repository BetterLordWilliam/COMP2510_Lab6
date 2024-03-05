#include <stdio.h>
#include <stdlib.h>

// Function Definitions
int* innitFiles(int *bitNum, int *ncount);
void getMax(long long *mxN, int *bN);
void getMin(long long *mnN, int *bN);
void write(int *arr, int *bN, long long *mxN, long long *mnN, int *c);

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
  long long maxN = 0, *mxN;
  long long minN = 0, *mnN; 

  cP = &count, bP = &bitNum; 
  mxN = &maxN, mnN = &minN;

  // Read file, get bits and numbers
  int *numbers = innitFiles(bP, cP);

  // Calculate max and min
  getMax(mxN, bP);
  getMin(mnN, bP);

  // Test stuff
  printf("%d %d\n", *bP, *cP);
  for (int i = 0; i != *cP; i++) {
    printf("%d ", numbers[i]);
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

void write(int *arr, int *bN, long long *mxN, long long *mnN, int *c) {
  // Print bounds and hex values if there are no numbers
  if (*c == 0) {
    fprintf(out, "min: %lld\t0x%016llx\nmax: %lld\t0x%016llx\n", *mnN, *mnN, *mxN, *mxN);
    return;
  } 
  
  long long prod = arr[0] * arr[1];

  int sign1 = ~(arr[0] >> *bN) + 1;
  int sign2 = ~(arr[1] >> *bN) + 1;

  // sign of number 1
  printf("sign0:%u\n", sign1);
  printf("sign1:%u\n", sign2);

  // Positive result, overflow risk
  if (sign1 == 0 && sign2 == 0 || sign1 == 1 && sign2 == 1) {
    printf("%x\t%x\n", *mxN, (prod));
    prod = ((*mxN - prod) & (1 << *bN)) ? *mxN : prod;
  // Negative result, underflow risk
  } else {
    printf("%x\t%x\n", *mnN, ~(prod-1));
    prod = (~(prod-1) & *mnN) ? *mnN : prod;
  } //    prod = (~(prod-1) & *mnN) ? *mnN : prod;

  printf("prod%lld\n", prod);
}