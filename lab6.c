// A01372608

#include <stdio.h>
#include <stdlib.h>

// Function Definitions
long long* innitFiles(int *bitNum, int *ncount);
void getMax(long long *mxN, int *bN);
void getMin(long long *mnN, int *bN);
void calcWrite(long long *arr, int *bN, long long *mxN, long long *mnN, int *c);
void printErrorExit();

FILE *in;
FILE *out;

/**
 * main:            Drives the program.
 *                  Assumes two operands are required.
 * param argc:      Argument count 
 * param *argv[]:   Arguments themselves 
*/
int main(int argc, char *argv[]) {
  // Lab setup
  if (argc != 3)
    return 1;

  in = fopen(argv[1], "r");
  out = fopen(argv[2], "w");

  // Bad file open
  if (in == NULL || out == NULL)
    return 1;

  int count = 0, *cP;
  int bitNum = 0, *bP;
  long long maxN = 0, *mxN;
  long long minN = 0, *mnN; 

  cP = &count, bP = &bitNum; 
  mxN = &maxN, mnN = &minN;

  // Read file, get bits and numbers
  long long *numbers = innitFiles(bP, cP);

  // Calculate max and min
  getMax(mxN, bP);
  getMin(mnN, bP);

  // Determine the output
  calcWrite(numbers, bP, mxN, mnN, cP);

  return 0;
}

/**
 * printErrorExit:    Prints 'exit' to out file and closes files. Terminates program.
*/
void printErrorExit() {
  if (out != NULL) {
    if (in != NULL) fclose(in);
    fprintf(out, "error\n");
    fclose(out);
  } else {
    printf("printErrorExit called incorrectly.\n");
  }
  exit(1);
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
  if (arr == NULL)
    printErrorExit();

  
  char buff[100];               // set buffer back to the beginning
  fseek(in, 0, SEEK_SET);
  fgets(buff, 100, in);
  sscanf(buff, "%d", bitNum);
  int sCount = 0;

  // Make sure the bit num is in the valid range
  if (*bitNum > 64 || *bitNum < 4)
    printErrorExit();

  // Get the numbers
  // Chose to assume that they would not be 2
  while (fgets(buff, 100, in) != NULL) {
    long long tempN = 0;
    int stat = sscanf(buff, "%lld\n", &tempN);    // Keep track of success

    if (stat == 1 && sCount < *ncount) {
      arr[sCount++] = tempN;

    } else {
      *ncount = (*ncount - 1);
    }
  }
  *ncount = (*ncount + 1);        // Reset after decrement of E

  if (*ncount == 1) printErrorExit();   // Exit if only one operand is presented
  if (*ncount > 2) printErrorExit();    // Exit if there are more than two operands

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
  *mxN = t;
}

/**
 * getMin:      Returns the minNumber for specified bitwidth.
 * param *mnN:  The address where this min is to be stored
 * param *bN:   The number of bits
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
  *mnN = t;
}

/**
 * abs_val:     Returns the absolute value of a long long.
 * param x:     long long, the number that we want the abs of
*/
long long abs_val(long long x) {
    long long mask = x >> (sizeof(long long) * 8 - 1);
    return (x ^ mask) - mask;
}

/**
 * calcWrite:   Calculates product of two integers, writes result to output file.
 * param arr:   Numbers that we want to determine their product
 * param bN:    The number of bits (max / min determiner)
 * param mxN:   The maximum bound
 * param mnN:   The minimum bound
 * param c:     The number of elements that we want to determine the product of
*/
void calcWrite(long long *arr, int *bN, long long *mxN, long long *mnN, int *c) {
  // Print bounds and hex values if there are no numbers
  // Not important to the calculation
  if (*c == 0) {
    fprintf(out, "min: %lld\t0x%016llx\nmax: %lld\t0x%016llx\n", *mnN, *mnN, *mxN, *mxN);
    return;
  }

  int signTotal = 0;

  // Lets try something else
  long long prod = 1;
  for (int i = 0; i != *c; i++) {
    prod *= arr[i];
    signTotal += ((arr[i] >> ((sizeof(long long) * 8)-1) &1));
  } // Determine product for the numbers

  // Sign bit of product
  int sign3 = (((prod) >> (*bN-1)) & 1);

  // Positive result, overflow risk
  if (signTotal % 2 == 0) {
    long long diff = abs_val(prod) - *mxN;
    long long is_a_greater = ~(diff >> (sizeof(long long) * 8 - 1)) & 1;

    if ((sign3) || is_a_greater)
      prod = *mxN;

  // Negative result, underflow risk
  } else {
    long long diff = abs_val(prod) - abs_val(*mnN);
    long long is_a_greater = ~(diff >> (sizeof(long long) * 8 - 1)) & 1;

    if ((!sign3) || is_a_greater)
      prod = *mnN;
  }
  
  // Put the product to the file
  fprintf(out, "%lld\n", (long long)prod);

  return;
}