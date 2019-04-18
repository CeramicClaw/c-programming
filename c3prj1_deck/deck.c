#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

void add_card_to(deck_t * deck, card_t c) {
  // Add the particular card to the given deck by realloc'ing the array of cards in that deck
  deck->n_cards++;
  deck->cards = realloc(deck->cards, deck->n_cards * sizeof(*deck->cards));
  deck->cards[deck->n_cards - 1] = malloc(sizeof(*deck->cards[deck->n_cards - 1]));
  deck->cards[deck->n_cards - 1]->value = c.value;
  deck->cards[deck->n_cards - 1]->suit = c.suit;
}

card_t * add_empty_card(deck_t * deck) {
  // Add a card whose value and suit are both 0 and return a pointer to it in the deck
  // This adds an invalid card as a placeholder for an unknown card
  card_t empty;
  empty.value = 0;
  empty.suit = 0;
  add_card_to(deck, empty);
  return deck->cards[deck->n_cards - 1];
}

deck_t * make_deck_exclude(deck_t * excluded_cards) {
  //Creates a deck that is full EXCEPT for all the cards that appear in excluded cards
  deck_t * ans = malloc(sizeof(*ans));
  ans->cards = malloc(sizeof(*ans->cards));
  ans->n_cards = 0;
  for (int i = 0; i < 52; i++) {
    if(deck_contains(excluded_cards, card_from_num(i)) == 1) {
      continue;
    }
    else {
      add_card_to(ans, card_from_num(i));
    }
  }
  return ans;
}

deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) {
  // Takes an array of hands and builds the deck of cards that remain after those cards have been removed from a full deck
  deck_t * excluded = malloc(sizeof(*excluded));
  excluded->cards = malloc(sizeof(*excluded->cards));
  excluded->n_cards = 0;
  for (int i = 0; i < n_hands; i++) {
    for(int j = 0; j < hands[i]->n_cards; j++) {
      if ((hands[i]->cards[j]->value == 0) && (hands[i]->cards[j]->suit == 0)) {
	continue;
      }
      else {
	add_card_to(excluded,*hands[i]->cards[j]);
      }
    }
  }
  deck_t * ans = make_deck_exclude(excluded);
  free_deck(excluded);
  return ans;
}

void free_deck(deck_t * deck) {
  for (int i = 0; i < deck->n_cards; i++) {
    free(deck->cards[i]);
  }
  free(deck->cards);
  free(deck);
}

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
