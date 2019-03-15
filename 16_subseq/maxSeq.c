#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t currentMax = 0;
  size_t maxLen = 0;
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      if (array[(i + 1)] > array[i]) {
	currentMax++;
      }
      else {
	currentMax++;
	if (currentMax > maxLen) {
	  maxLen = currentMax;
	  currentMax = 0;
	}
	else {
	  currentMax = 0;
	}
      }
    }
  }
  else {
    return maxLen;
  }
  return maxLen;
}

//int main (void) {
//  int testArray[] = {-1, 0, 1, 0, 5, 8, 13, 2, 4};
//  unsigned maxLen = maxSeq(testArray, 9);
//  printf("Max length is %u\n", maxLen);
//  return EXIT_SUCCESS;
//}
