#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

//char * stripNewline(char * line)

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  counts_t * ans = createCounts();
  FILE * f = fopen(filename, "r");
  char * line = NULL;
  char * key = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0 ) {
    key = malloc(strlen(line) * sizeof(*key));
    strncpy(key,line,(strlen(line) - 1));
    key[strlen(line) - 1] = '\0';
    addCount(ans,lookupValue(kvPairs, key));
    free(key);
  }
  if (fclose(f) != 0) {
    fprintf(stderr, "Unable to close file\n");
  }
  free(line);
  return ans;
}

char * foutName(char * filename) {
  const char * end = ".ans";
  int len = strlen(filename) + strlen(end) + 1;
  char * outName = malloc(len * sizeof(*outName));
  snprintf(outName, len, "%s%s", filename, end);
  return outName;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
 //read the key/value pairs from the file named by argv[1] (call the result kv)
  if(argc < 2) {
    fprintf(stderr, "Requires 2 input files.\n");
    return EXIT_FAILURE;
  }
  kvarray_t * kv = readKVs(argv[1]);
  //printKVs(kv);
 //count from 2 to argc (call the number you count i)
  for(int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kv);
    //compute the output file name from argv[i] (call this outName)
    char * outName = foutName(argv[i]);
    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w+");
    //print the counts from c into the FILE f
    printCounts(c, f);
    //close f
    if (fclose(f) != 0) {
      fprintf(stderr, "Could not close file.\n");
      return EXIT_FAILURE;
    }
    freeCounts(c);
    free(outName);
    //free the memory for outName and c
  }
 //free the memory for kv
  freeKVs(kv);
  return EXIT_SUCCESS;
}
