#include <stdio.h>
#include <stdlib.h>
#include "future.h"

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
  //printf("Index = %zu\n", index);
  if(index + 1 > (fc->n_decks)) {
    fc->decks = realloc(fc->decks, (index + 1) * sizeof(*(fc->decks)));
    for(size_t i = fc->n_decks; i < (index + 1); i++) {
      (fc->decks[i]).n_cards = 0;
      (fc->decks[i]).cards = NULL;
    }
    fc->n_decks = index + 1;
  }
  //printf("fc->n_decks = %zu\n", fc->n_decks);
  (fc->decks[index]).n_cards++;
  //printf("(fc->decks[index]).n_cards = %zu\n", (fc->decks[index]).n_cards);
  (fc->decks[index]).cards = realloc((fc->decks[index]).cards, (fc->decks[index]).n_cards * sizeof(*((fc->decks[index]).cards)));
  (fc->decks[index]).cards[(fc->decks[index]).n_cards - 1] = ptr;
}

void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
  if(deck->n_cards < fc->n_decks) {
    fprintf(stderr,"Input deck not large enough.");
    exit(EXIT_FAILURE);
    return;
  }
  for(int i = 0; i < fc->n_decks; i++) {
    for (int j = 0; j < (fc->decks[i]).n_cards; j++) {
      (fc->decks[i]).cards[j]->value = deck->cards[i]->value;
      (fc->decks[i]).cards[j]->suit = deck->cards[i]->suit;
      assert_card_valid(*(fc->decks[i]).cards[j]);
    }
  }
}
