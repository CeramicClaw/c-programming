#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    printf("Incorrect! %d ^ %d expected %d actual %d\n", x, y, expected_ans, power(x,y));
    exit(EXIT_FAILURE);
  }
}

int main (void) {
  run_check(0, 1, 0);
  run_check(0, 0, 1);
  run_check(-2, 2, 4);
  run_check(-2, 3, -8);
  unsigned test = power(-2, -1);
  printf("%d\n",test);
  return EXIT_SUCCESS;
}
