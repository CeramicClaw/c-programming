#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
  //Allocate inputname + 1 + 7
  char * outputName = malloc((strlen(inputName) + 8)  * sizeof(outputName));
  strcpy(outputName, inputName);
  outputName = strcat(outputName, ".counts");
  return outputName;
}
