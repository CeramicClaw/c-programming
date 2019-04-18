#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int card_ptr_comp(const void * vp1, const void * vp2) {
  /*Sort hands by value in descending order  using quicksort
    If they have the same value, sort by suit: club < diamond < heart < spade
   */
  const card_t * const * cp1 = vp1;
  const card_t * const * cp2 = vp2;
  
  if ((**cp1).value > (**cp2).value) {
    return -1;
  }
  else if ((**cp1).value < (**cp2).value) {
    return 1;
  }
  else {
    if ((**cp1).suit > (**cp2).suit) {
      return -1;
    }
    else if ((**cp1).suit < (**cp2).suit) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

suit_t flush_suit(deck_t * hand) {
  /* Looks at hand and determines if it is a flush
     If so, it returns the suit of the cards comprising the flush (SPADES, HEARTS, etc.)
     If not, it returns NUM_SUITS
   */
  int n_spades = 0;
  int n_hearts = 0;
  int n_diamonds = 0;
  int n_clubs = 0;
  for(int i = 0; i < (*hand).n_cards; i++) {
    switch ((*(*hand).cards[i]).suit) {
    case SPADES: n_spades++; break;
    case HEARTS: n_hearts++; break;
    case DIAMONDS: n_diamonds++; break;
    case CLUBS: n_clubs++; break;
    default: break;
    }
  }
  if (n_spades >= 5) { return SPADES; }
  if (n_hearts >=5) { return HEARTS; }
  if (n_diamonds >=5) { return DIAMONDS; }
  if (n_clubs >=5) { return CLUBS; }
  return NUM_SUITS;
}

unsigned get_largest_element(unsigned * arr, size_t n) {
  // Returns largest element in an array of unsigned integers
  if (n == 1) {
    return arr[0];
  }
  unsigned largest = arr[0];
  for (int i = 0; i < (n - 1); i++) {
    if (arr[i + 1] > largest) {
      largest = arr[i + 1];
    }
  }
  return largest;
}

size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind){
  /* Returns the index in the array (match_counts) whose value is n_of_akind
     The array has n elements
     If there are multiple values equal to n_of_akind, return the LOWEST index
  */
  for (int i = 0; i < n; i++) {
    if (match_counts[i] == n_of_akind) {
      return i;
    }
  }
  return 0;
}
ssize_t  find_secondary_pair(deck_t * hand,
			     unsigned * match_counts,
			     size_t match_idx) {
  /* Look for another pair if there is a 3 of a kind or a pair
     Takes in the hand, match counts from before, and the index of the original match
     It should find the index of a card under the following conditions:
       - Match count is > 1 (i.e. at least a pair)
       - Card value is not the same as value of the card at match_idx
       - It is the lowest index meeting the first 2 conditions
     If no such index is found, return -1
  */  
  for (int i = 0; i < (*hand).n_cards; i++) {
    if (match_counts[i] > 1) {
      if (i == match_idx) {
	i += (match_counts[i] - 1);
	continue;
      }
      return i;
    }
  }
  return -1;
}

int is_n_length_straight_at(deck_t * hand, size_t index, suit_t fs, int n) {
  card_t card_to_find;
  int search_len = n;
  int straight_len = 1;
  card_to_find.value = (*(*hand).cards[index]).value;
  if (fs == NUM_SUITS) {
    //Only worry if the next card is lower in value, suit doesn't matter
    for (int i = 0; i < ((*hand).n_cards - index - 1)  ; i++) {
      if ( (*(*hand).cards[index + i]).value == (*(*hand).cards[index + i + 1]).value) {
	search_len++;
	continue;
      }
      if ( ((*(*hand).cards[index + i]).value) == ((*(*hand).cards[index + i+1]).value + 1) ) {
	straight_len++;
	continue;
      }
      else if (straight_len >= n) {
	return 1;
      }
      else {
	return 0;
      }
    }
    if (straight_len >= n) {
	return 1;
      }
  }
  else {
    //Card has to match suit
    card_to_find.suit = fs;
    if (card_to_find.suit != (*(*hand).cards[index]).suit) {
      return 0;
    }
    int len = 0;
    for (int i = 0; i < n; i++) {
      if (deck_contains(hand, card_to_find) == 1) {
	  len++;
	  card_to_find.value--;
      }
    }
    if (len >= n){
      return 1;
    }
    return 0;
  }
  return 0;
}

int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs) {
  if ( (*(*hand).cards[index]).value == 14  ) {
    if (fs == NUM_SUITS) {
      for (int i = 0; i < (*hand).n_cards; i++) {
	if ( (*(*hand).cards[i]).value == 5) {
	  return is_n_length_straight_at(hand, i, fs, 4);
	}
      }
    }
    else {
      for (int i = 0; i < (*hand).n_cards; i++) {
	if ( ((*(*hand).cards[i]).value == 5) && ( (*(*hand).cards[i]).suit == fs ) ) {
	  return is_n_length_straight_at(hand, i, fs, 4);
	}
      }
    }
  }
  return 0;
}

int is_straight_at(deck_t * hand, size_t index, suit_t fs) {
  /* Determine if there is a straight starting at index (and only index) in the given hand
     If fs = NUM_SUITS, then it should look for any straight
     If fs = anyhing else, look for a straight flush and return:
     - `-1` if an Ace-low straight was found at that index
     - `0` if no straight was found at that index
     - `1` if any other straight was found at that index
     
     Assume the hand is sorted by value in descending order (A, K, Q, ...)
  */
  if ( is_n_length_straight_at(hand, index, fs, 5) == 1 ) {
    return 1;
  }
  if ( is_ace_low_straight_at(hand, index, fs) == 1 ) {
    return -1;
  }
  return 0;
}



hand_eval_t build_hand_from_match(deck_t * hand,
				  unsigned n,
				  hand_ranking_t what,
				  size_t idx) {
  /* Construct a hand_eval_t which has the ranking and 5 cards used for it
     It should make a hand_eval_t and set its ranking to the passed "what" value,
     copy "n" cards from the hand, starting at "idx" in to the first "n" elements fo the hand_eval_t "cards" array
   */
  hand_eval_t ans;
  ans.ranking = what;
  if (n == 0) {
    for (int i = 0; i < 5; i++) {
      ans.cards[i] = (*hand).cards[i];
    }
    return ans;
  }
  
  for (int i = 0; i < n; i++) {
    ans.cards[i] = (*hand).cards[idx + i];
  }
  int cards_in_ans = n;
  for  (int i = 0; cards_in_ans < 5; i++) {
    if (i == idx) {
      i += (n - 1);
      continue;
    }
    ans.cards[cards_in_ans] = (*hand).cards[i];
    cards_in_ans++;
  }
  return ans;
}


int compare_hands(deck_t * hand1, deck_t * hand2) {
  /* 1. Sort each hand using qsort on the hand's cards and card_ptr_comp
     2. Call evaluate_hand on each hand that returns a hand_eval_t
     3. Check if rankings in hand_eval_t are the same or different and use the ranking to determine a winner
     4. If they are the same, look at the values in the cards array of each hand_eval_t to break the tie. Compare element 0 of each hand, then element 1, etc. until reaching the end
     5. If hand 1 is better, return 1. If hand 2, return -1. If tie, return 0
  */
  qsort((*hand1).cards, (*hand1).n_cards, sizeof(const card_t * const *), card_ptr_comp);
  qsort((*hand2).cards, (*hand2).n_cards, sizeof(const card_t * const *), card_ptr_comp);
  hand_eval_t hand1_eval = evaluate_hand(hand1);
  hand_eval_t hand2_eval = evaluate_hand(hand2);
  if (hand1_eval.ranking < hand2_eval.ranking) {
    return 1;
  }
  else if (hand1_eval.ranking > hand2_eval.ranking) {
    return -1;
  }
  for (int i = 0; i < 5; i++) {
    if ( (*hand1_eval.cards[i]).value > (*hand2_eval.cards[i]).value) {
      return 1;
    }
    else if ((*hand1_eval.cards[i]).value < (*hand2_eval.cards[i]).value) {
      return -1;
    }
    else {
      continue;
    }
  }
  return 0;
}



//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
unsigned * get_match_counts(deck_t * hand) {
  for (int i = 0; i < hand->n_cards; i++) {
    assert_card_valid(*hand->cards[i]);
  }
  unsigned * counts = malloc(hand->n_cards * sizeof(*counts));
  for (int i = 0; i < hand->n_cards; i++) {
    counts[i] = 0;
  }
  for (int i = 0; i < hand->n_cards; i++) {
    counts[i]++;
    for(int j = i + 1; j < hand->n_cards; j++) {
      if(hand->cards[i]->value == hand->cards[j]->value) {
	counts[i]++;
	counts[j]++;
      }
    }
  }
  return counts;
}

// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
	(fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}


//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
      if (x < 0) { //ace low straight
	assert(hand->cards[i]->value == VALUE_ACE &&
	       (fs == NUM_SUITS || hand->cards[i]->suit == fs));
	ans->cards[4] = hand->cards[i];
	size_t cpind = i+1;
	while(hand->cards[cpind]->value != 5 ||
	      !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
	  cpind++;
	  assert(cpind < hand->n_cards);
	}
	copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
	copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}


//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {
  suit_t fs = flush_suit(hand);
  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
  unsigned * match_counts = get_match_counts(hand);
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);
  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
	ans.cards[copy_idx] = hand->cards[i];
	copy_idx++;
	if (copy_idx >=5){
	  break;
	}
      }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}
