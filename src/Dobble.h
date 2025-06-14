#pragma once
#include <stdint.h>
#include <vector>


typedef uint32_t SymbolId;

struct Symbol {
    const char *str;
};

// A card is holding a number of symbols given by their id
typedef std::vector<SymbolId> Card;

// A deck holds several cards
typedef std::vector<Card> CardDeck;


struct CardDeckMetrics {
    uint32_t Num_Cards;
    uint32_t Num_Symbols_per_Card;
    uint32_t Num_Symbols;
};

bool has_no_duplicate_symbols(const Card& c);
uint32_t countCommonSymbols(const Card& c1, const Card& c2, bool fast=true); 
bool checkCardAgainstDeck(const Card& card, const CardDeck::iterator& begin, const CardDeck::iterator& end,
     const Card* skipCard=nullptr, bool failure_only_duplicate_symbols=false); 
bool validateCardDeck(CardDeck& deck);


void printCardDeckAnalysis(CardDeck& deck);


#ifdef DEBUG
void print(const Card& card);
void println(const Card& card);
#endif