#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

size_t  runTest(int * array, size_t n, size_t expected) {
  size_t test = maxSeq(array, n);
  if (test != expected){
    return 0;
  }
  else {
    return 1;
  }
}

int main (void) {
  size_t sumTest = 0;
  int array1[] = {5, -1, 0, 5, 10};
  int array2[] = {1, 2, 2, 4, 5, 6, 6, 4, 6};
  int array3[] = {0};
  int array4[] = {};
  sumTest = (runTest(array1, 5, 4) + runTest(array2, 9, 4) + runTest(array3, 1, 1) + runTest(array4, 0, 0));
  if (sumTest == 4){
    return EXIT_SUCCESS;
  }
  else {
    return EXIT_FAILURE;
  }
}
