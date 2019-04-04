#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

void print_hand(deck_t * hand){
  // Print each card and put a space after each card. Do not put a newline.
  for (int i = 0; i < (*hand).n_cards; i++) {
    print_card(*(*hand).cards[i]);
    printf(" ");
  }
}

int deck_contains(deck_t * d, card_t c) {
  // Check if a deck contains a particular card. If so, return 1. Else, return 0;
  for (int i = 0; i < (*d).n_cards; i++) {
    if ( ((*(*d).cards[i]).value == c.value) && ((*(*d).cards[i]).suit == c.suit) ) {
      return 1;
    }
  }
  return 0;
}

void shuffle(deck_t * d){
  //Shuffle deck using random();
  card_t card_0;                  //holder for card_0
  int n_cards = (*d).n_cards;       //total number of cards to shuffle
  for (int i = 0; i < 10; i++) {
    int randPos = (int)(random() % n_cards);
    card_0 = *(*d).cards[0];
    for (int j = 0; j < randPos; j++) {
      *(*d).cards[j] = *(*d).cards[j+1];
      }
    *(*d).cards[randPos] = card_0;
  }
}

void assert_full_deck(deck_t * d) {
  //Check if deck contains every valid card once
  //Hint, use deck_contains();
  for (int i = 0; i < (*d).n_cards; i++) {
    assert (deck_contains(d, *(*d).cards[i]) );
  }
}
