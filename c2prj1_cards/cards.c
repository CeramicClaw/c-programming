#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cards.h"


void assert_card_valid(card_t c) {
  //use assert() to check that the card passed has valid values
  //It should be between 2 and VALUE_ACE (inclusive)
  //And its suit should be between SPADES and CLUBS (inclusive)
  assert( (c.value >= 2) && (c.value <= VALUE_ACE) );
  assert( (c.suit >= SPADES) && (c.suit <= CLUBS) );
}

const char * ranking_to_string(hand_ranking_t r) {
  //Convert hand_ranking_t enumerated value into a string that describes this
  switch (r) {
  case STRAIGHT_FLUSH: return "STRAIGHT_FLUSH"; break;
  case FOUR_OF_A_KIND: return "FOUR_OF_A_KIND"; break;
  case FULL_HOUSE: return "FULL_HOUSE"; break;
  case FLUSH: return "FLUSH"; break;
  case STRAIGHT: return "STRAIGHT"; break;
  case THREE_OF_A_KIND: return "THREE_OF_A_KIND"; break;
  case TWO_PAIR: return "TWO_PAIR"; break;
  case PAIR: return "PAIR"; break;
  case NOTHING: return "NOTHING"; break;
  default: return "???"; break;
  }
}

char value_letter(card_t c) {
  //Return the character that textually represents the value of the passed-in card
  //For 2-9 it should be that digit
  //For 10 it should be 0
  //For Jack"; break; Queen"; break; King, Ace it should be 'J', 'Q', 'K', 'A'
  if ( (c.value >= 2) && (c.value <= 9) ) {
    return (48 + c.value);
  }
  else {
    switch (c.value) {
    case 10: return 48; break;
    case VALUE_JACK: return 74; break;
    case VALUE_QUEEN: return 81; break;
    case VALUE_KING: return 75; break;
    case VALUE_ACE: return 65; break;
    default: return 'x'; break;
    }
  }
}


char suit_letter(card_t c) {
  //Return the letter that textually represents the suit
  //i.e. 's', 'h', 'd', or 'c' for SPADES, HEARTS, etc.  
  switch (c.suit) {
  case SPADES: return 's'; break;
  case HEARTS: return 'h'; break;
  case DIAMONDS: return 'd';break;
  case CLUBS: return 'c'; break;
  default: return 'x'; break;
  }
}

void print_card(card_t c) {
  //Print the textual representation of the card
  //'As' for 'Ace of spades', '0d' for '10 of diamonds', etc.
  //Do not print any additional spaces or newlines after the card text
  printf("%c%c", value_letter(c), suit_letter(c));
}

card_t card_from_letters(char value_let, char suit_let) {
  //Make and return a card_t whose value and suit correspond to the letters passed in
  //If the values passed are invalid, use assert() or print an error message and EXIT_FAILURE
  card_t temp;
  if ( ((value_let - 48) >= 2) && ((value_let - 48) <= 9) ) {
    temp.value = (value_let - 48);
  }
  else {
    switch (value_let) {
    case '0': temp.value = 10; break;
    case 'J': temp.value = VALUE_JACK; break;
    case 'Q': temp.value = VALUE_QUEEN; break;
    case 'K': temp.value = VALUE_KING; break;
    case 'A': temp.value = VALUE_ACE; break;
    default: temp.value = 'x'; break;
    }
  }

  switch (suit_let) {
    case 's': temp.suit = SPADES; break;
    case 'h': temp.suit = HEARTS; break;
    case 'd': temp.suit = DIAMONDS; break;
    case 'c': temp.suit = CLUBS; break;
    default: temp.suit = 9; break;
  }

  assert_card_valid(temp);
  return temp;
}

card_t card_from_num(unsigned c) {
  //Take a number from 0 (inclusive) to 52 (exclusive)
  //Map it uniquely to a card value/suit combination
  card_t temp;
  if (c < 13) {
    temp.suit = SPADES;
  }
  else if (c < 26) {
    temp.suit = HEARTS;
  }
  else if (c < 39) {
    temp.suit = DIAMONDS;
  }
  else {
    temp.suit = CLUBS;
  }
  temp.value = ((c % 13) + 2);
  assert_card_valid(temp);
  return temp;
}
