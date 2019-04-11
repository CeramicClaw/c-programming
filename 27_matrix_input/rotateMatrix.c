#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Program will take ONE command line argument: a string specifying an input file

   Input file should contain 10 lines, each of which have 10 (non-newline) characters, plus a newline

   Program will rotate this 90 degrees clockwise and print the result on stdout

   If there are any errors, print an appropriate message to stderr and exit with EXIT_FAILURE

   Use strchr for error checking that is is a proper line

*/

void rotate(char matrix[10][10]) {
  char newMatrix[10][10];
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      newMatrix[i][j] = matrix[9-j][i];
    }
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      matrix[i][j] = newMatrix[i][j];
    }
  }
}

void clearMatrix(char * matrix) {
  for (int i = 0; i < 12; i++) {
    matrix[i] = 0;
  }
}

void printMatrix(char matrix[10][10]) {
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      printf("%c",matrix[i][j]);
    }
    printf("\n");
  }
}

int main (int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide an input file.\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file.\n");
    return EXIT_FAILURE;
  }
  char line[12];
  int num_lines = 0;
  char matrix[10][10];
  while ( fgets(line, 12, f) != NULL ) {
    if ( (strchr(line, '\n') == NULL) || (line[10] != '\n') ) {
      printf("Incorrect line size!\n");
      return EXIT_FAILURE;
    }
    if (num_lines > 9) {
      printf("Too many lines!\n");
      return EXIT_FAILURE;
    } 
    for(int i = 0; i < 10; i++) {
      matrix[num_lines][i] = line[i];
    }
    num_lines++;
    clearMatrix(line);
  }
  if (num_lines < 9) {
    printf("Too few lines!\n");
    return EXIT_FAILURE;
  }
  if(fclose(f) != 0) {
    perror("File did not close correctly.\n");
    return EXIT_FAILURE;
  }
  rotate(matrix);
  printMatrix(matrix);
  return EXIT_SUCCESS;
}
