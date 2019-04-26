#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"

int * initializeWinArray(size_t n_hands) {
  int * winArray = malloc((n_hands + 1) * sizeof(*winArray));
  for(int i = 0; i < (n_hands + 1); i++) {
    winArray[i] = 0;
  }
  return winArray;
}

void free_fc(future_cards_t * fc) {
  for(int i = 0; i < fc->n_decks; i++) {
    free((fc->decks[i]).cards);
  }
  free(fc->decks);
  free(fc);
}

void free_hands(deck_t ** hands, size_t n_hands) {
  for(int i = 0; i < n_hands; i++) {
    for(int j = 0; j < hands[i]->n_cards; j++) {
      free(hands[i]->cards[j]);
    }
    free(hands[i]->cards);
    free(hands[i]);
  }
  free(hands);
}

int findWinningHand (deck_t ** hands, size_t n_hands) {
  //Return the index of the winning hand; if everything is a tie, return n_hands
  if(n_hands == 1) {
    return 0;
  }
  int index = 0;
  int ties = 0;
  for(int i = 0; i < (n_hands - 1); i++) {
    if (compare_hands(hands[index], hands[i+1]) == 0) {
      ties++;
    }
    if (compare_hands(hands[index], hands[i+1]) < 0) {
      index = i + 1;
    }
  }
  if(ties == (n_hands - 1)) {
    return n_hands;
  }
  return index;
}

int findWinner(int * winArray, size_t n_hands) {
  size_t index = 0;
  for (int i = 0; i < (n_hands - 1); i++) {
    if(winArray[index] < winArray[i + 1]) {
      index = i + 1;
    }
  }
  return index;
}

void printWins(int * winArray, size_t n_hands, int num_trials) {
  int winner = findWinner(winArray, n_hands);
  float winPct = 100* ((float)winArray[winner] / (float)num_trials);
  printf("Hand %u won %u / %u times (%.2f%%)\n", winner, winArray[winner], num_trials, winPct);
  printf("And there were %u ties\n", winArray[n_hands]);
}

int main(int argc, char ** argv) {
  
  // 1. Check command line arguments/report errors
  if(argc < 2) {
    fprintf(stderr, "Please provide input file.\n");
    return EXIT_FAILURE;
  }
  // 2. Open input file & read hands using read_input
  future_cards_t * fc = malloc(sizeof(*fc));
  fc->decks = malloc(sizeof(*(fc->decks)));
  fc->n_decks = 0;
  size_t n_hands = 0;
  FILE * f = fopen(argv[1], "r"); 
  deck_t ** hands = read_input(f, &n_hands, fc);
  // 3. Create a deck with the remaining cards using build_remaining_deck
  deck_t * remaining_deck = build_remaining_deck(hands, n_hands);
  // 4. Create an array to count how many times each hand wins + 1 if there is a tie (for 2 hands, 3 elements; initialize all values to 0)
  int * winArray = initializeWinArray(n_hands);
  int num_trials = 0;
  if (argc == 2) {
    num_trials = 10000;
  }
  else {
    if (isdigit(*argv[2])) {
      num_trials = atoi(argv[2]);
    }
    else {
      fprintf(stderr, "Invalid number of trials.\n");
      return EXIT_FAILURE;
    }
  }
  /* 5. Do each Monte Carlo trial (repeat num_trials times)
       - Shuffle deck of remaining cards
       - Assign unknown cards using future_cards_from_deck
       - Use compare_hands to see what hand won (for more than two hands it is like finding the max of an array
       - Increment win count for winning hand (or "ties" if a tie)
  */
  for(int i = 0; i < num_trials; i++) {
    shuffle(remaining_deck);
    future_cards_from_deck(remaining_deck, fc);
    winArray[findWinningHand(hands, n_hands)]++;
  }
  // 6. Print results
  printWins(winArray, n_hands, num_trials);
  //  7. Free memory
  free(winArray);
  free_deck(remaining_deck);
  free_fc(fc);
  free_hands(hands, n_hands);
  //  8. Close any open files
  if(fclose(f) != 0) {
    fprintf(stderr, "Error closing file.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
