#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Take one command line argument: the name of the file to be read

   Perform frequency counting on the letters. Ignore all non-letter characters

   Determine which letter is 'e' and solve for the key

   On success, print a single line of output containing one decimal integer followed by a newline in range [0, 26)

   On failure, provide an error message and EXIT_FAILURE

*/


int largestElem(int * array, int size) {
  // Returns the index of the largest element in the array
  int index = 0;
  for (int i = 0; i < (size - 1); i++) {
    if (array[i + 1] > array[index]) {
	index = i + 1;
    }
  }
  return index;
}
  

char frequency(FILE * f) {
  //Peforms frequency analysis on stream f and returns the most common char
  int freq[26] = {0}; //Array to count frequency
  int c;
  while( (c=fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      c -= 'a';
      freq[c]++;
    }
  }
  int index = largestElem(freq, 26);
  return (index + 'a');
}

int convert_to_e(char c) {
  if (c >= 'e') {
    return (c - 'e');
  }
  return ('e' - c + 18);
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide an input file.\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file.\n");
    return EXIT_FAILURE;
  }
  char c = frequency(f); //The most common letter in the file. Assume it is 'e'
  printf("Most common letter is '%c'.\n",c);
  printf("%d\n",convert_to_e(c));
  return EXIT_SUCCESS;
}
