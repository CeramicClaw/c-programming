#include <stdio.h>
#include <stdlib.h>

unsigned power_helper(unsigned x, unsigned y, unsigned ans) {
  if (y == 1) {
    return ans;
  }
  else {
    return power_helper(x, y-1, x * ans);
  }
}

unsigned power(unsigned x, unsigned y){
  if (y == 0) {
    return 1;
  }
  if (y < 0) {
    x = (1/x);
    y *= -1;
  }
  return power_helper(x, y, x);
}
