#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void printLines(char ** lineArray, size_t numLines) {
  for (size_t i = 0; i < numLines; i++) {
    printf("%s", lineArray[i]);
    free(lineArray[i]);
  }
  free(lineArray);
}

char ** copyLines(FILE * f, char ** lineArray, size_t * numLines) {
  /*  Takes a file stream (f), the desired line array (lineArray), and a pointer to number of lines
    Allocates memory and stores each line in lineArray, updating numLines, and
    Returns array
  */
  char * curr_line = NULL;
  size_t size = 0;
  while(getline(&curr_line, &size, f) >= 0) {
    *numLines = *numLines + 1;
    lineArray = realloc(lineArray, (*numLines * sizeof(*lineArray)));
    lineArray[*numLines - 1] = curr_line;
    curr_line = NULL;
  }
  free(curr_line);
  return lineArray;
}

int main(int argc, char ** argv) {
  char ** lineArray = NULL;
  size_t numLines = 0;
  /*If argc == 1: Read from standard input, Sort the lines, Print the results/Free memory, Exit successfully
  */
  if(argc == 1) {
    lineArray = copyLines(stdin, lineArray, &numLines);
    sortData(lineArray, numLines);
    printLines(lineArray, numLines);
  }
  /*If argc > 1
    Treat each argument as an input file. 
    For each file: 
    Open file, Read all lines, sort the lines, print the results/Free memory, Close file
    Repeat until no more files
    If an error occurs, print an appropriate error message and exit with EXIT_FAILURE
  */
  if(argc > 1) {
    for(int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
	fprintf(stderr, "Could not open file\n");
	return EXIT_FAILURE;
      }
      lineArray = copyLines(f, lineArray, &numLines);
      sortData(lineArray, numLines);
      printLines(lineArray, numLines);
      if (fclose(f) != 0) {
	fprintf(stderr, "File did not close properly.\n");
	return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
