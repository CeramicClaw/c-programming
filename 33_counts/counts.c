#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
  /* Allocate memory for a counts_t structure
     Initialize it to represent nothing has been counted
  */
  counts_t * new = malloc(sizeof(*new));
  new->countArray = malloc(sizeof(*new->countArray));
  //new->countArray[0]->str = malloc(sizeof(*new->countArray[0]->str));
  new->numCounts = 0;
  new->numUnknown = 0;
  return new;
}

int checkCount(counts_t * c, const char * name) {
  for(int i = 0; i < c->numCounts; i++) {
    if(strcmp(c->countArray[i]->str, name) == 0) {
      return 1;
    }
  }
  return 0;
}

void addCount(counts_t * c, const char * name) {
  /* Increment count for the corresponding name
     Name will be NULL in the case of something unknown
  */
  if (name == NULL) {
    c->numUnknown++;
    return;
  }
  if(checkCount(c, name) == 1) {
    for(int i = 0; i < c->numCounts; i++) {
      if (strcmp(c->countArray[i]->str, name) == 0) {
	c->countArray[i]->numTimes++;
      }
    }
  }
  else {
    c->numCounts++;
    c->countArray = realloc(c->countArray, c->numCounts * sizeof(*c->countArray));      //Reallocate additional *one_count_t
    c->countArray[c->numCounts - 1] = malloc(sizeof(*c->countArray[c->numCounts - 1]));  //Allocate additional one_count_t
    c->countArray[c->numCounts - 1]->str = malloc((strlen(name) + 1) * sizeof(*name));  //Allocate space for str
    strcpy(c->countArray[c->numCounts - 1]->str,name);
    c->countArray[c->numCounts - 1]->numTimes = 1;
  }
}
void printCounts(counts_t * c, FILE * outFile) {
  /* Prints a counts_t to FILE outfile in the order that the name is first added with unknown always last
     NEVER print unknown if there are no unknown values
     Format is:  'Captain: #' OR '<unknown> : #'
  */
  for(int i = 0; i < c->numCounts; i++) {
    printf("%s: %d\n",c->countArray[i]->str, c->countArray[i]-> numTimes);
  }
  if(c->numUnknown > 0) {
    printf("<unknown> : %d\n",c->numUnknown);
  }
}

void freeCounts(counts_t * c) {
  // Free all memory associated with counts_t
  for (int i = 0; i < c->numCounts; i++) {
    free(c->countArray[i]->str);
    free(c->countArray[i]);
  }
  free(c->countArray);
  free(c);
}
