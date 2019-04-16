#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"

kvpair_t * lineToKV(char * line) {
  /* Take a single line of text and convert it into a key/value pair
     Search for first occurence of '=' and split
  */
  kvpair_t * ans = malloc(sizeof(*ans));
  char * p = strchr(line, '=');
  int dist = p - line;
  ans->key = malloc( (dist + 1) * sizeof(*ans->key));
  strncpy(ans->key, line, dist);
  ans->key[dist] = '\0';
  ans->value = malloc( (strlen(line) -  dist - 1) * sizeof(*ans->value));
  strncpy(ans->value, (p+1), (strlen(line) - dist - 2));
  ans->value[(strlen(line) - dist - 2)] = '\0';
  return ans;
}

kvarray_t * readKVs(const char * fname) {
  /* Open the file
     read lines of text
     split them into key/value pairs
     add resulting pairs to a realloc' array
     close file
     Return the kvarray_t * that has the array
  */
  FILE * f = fopen(fname, "r");
  kvarray_t * ans = malloc(sizeof(*ans));
  ans->KVs = NULL;
  char * line = NULL;
  size_t sz = 0;
  int nLines = 0;
  while (getline(&line, &sz, f) >= 0) {
    ans->KVs = realloc(ans->KVs, (nLines + 1) * sizeof(*ans->KVs));
    ans->KVs[nLines] = lineToKV(line);
    nLines++;
  }
  free(line);
  ans->num_KVs = nLines;
  if (fclose(f) != 0) {
    fprintf(stderr, "File did not close correctly\n");
  }
  return ans;
}

void freeKVs(kvarray_t * pairs) {
  //Free all memory allocated by readKVs
  for(int i = 0; i < pairs->num_KVs; i++) {
    free(pairs->KVs[i]->key);
    free(pairs->KVs[i]->value);
    free(pairs->KVs[i]);
  }
  free(pairs->KVs);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //Print all KV pairs in a given kvarray_t in the format "key = '%s' value = '%s'\n"
  for (int i = 0; i < pairs->num_KVs; i++) {
    printf("key = '%s' value = '%s'\n", pairs->KVs[i]->key, pairs->KVs[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //Search 'pairs' for matching 'key' and, if found, return the corresponding value
  //If no match is found, return NULL
  for (int i = 0; i < pairs->num_KVs; i++) {
    if ( strcmp(pairs->KVs[i]->key,key) == 0) {
      return pairs->KVs[i]->key;
    }
  }
  return NULL;
}
