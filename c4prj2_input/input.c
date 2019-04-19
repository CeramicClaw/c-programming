#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "future.h"

deck_t * hand_from_string(const char * str, future_cards_t * fc) {
  deck_t * ans = malloc(sizeof(*ans));
  ans->cards = malloc(sizeof(*ans->cards));
  ans->n_cards = 0;
  int n_cards = 0;
  //printf("Line length: %zu\n", strchr(str, '\n') - str);
  int len = strchr(str, '\n') - str;
  for(int i = 0; i < len; i = i+3) {
    //print_card(card_from_letters(str[i], str[i+1]));
    if (str[i] == '?') {
      add_future_card(fc,(str[i+1] -48),add_empty_card(ans));
      continue;
    }
    else {
      add_card_to(ans, card_from_letters(str[i],str[i+1]));
    }
  }
  return ans;
}

deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc) {
  deck_t ** ans = malloc(sizeof(*ans));
  char * line = NULL;
  size_t sz = 0;
  if( f == NULL) {
    fprintf(stderr,"Cannot open file.\n");
    exit(EXIT_FAILURE);
  }
  while(getline(&line, &sz, f) >= 0) {
    *n_hands++;
    ans = realloc(ans, *n_hands * sizeof(*ans));
    ans[*n_hands - 1] = hand_from_string(line, fc);
    //check if hand has at least 5 cards
  }
  free(line);
  return ans;
}


// DELETE THIS WHEN SUBMITTING
//int main(void) {
//char * str = "As Jd Kh ?0 ?1 ?2\n";
//future_cards_t * fc = malloc(sizeof(*fc));
//fc->decks = malloc(sizeof(*fc->decks));
//fc->n_decks = 0;
//deck_t * deck = hand_from_string(str, fc);
//return EXIT_SUCCESS;
//}
