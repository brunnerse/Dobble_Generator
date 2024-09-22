#pragma once
#include <stdint.h>
#include <vector>


typedef uint32_t SymbolId;

struct Symbol {
    SymbolId id;
    const char *str;
};

// A card is holding a number of symbols given by their id
typedef std::vector<SymbolId> Card;

typedef std::vector<Card> CardDeck;


struct CardDeckMetrics {
    uint32_t Num_Cards;
    uint32_t Num_Symbols_per_Card;
    uint32_t Num_Symbols;
};

uint32_t countCommonSymbols(const Card& c1, const Card& c2, bool fast=true); 
bool checkCardAgainstDeck(const Card& card, const CardDeck::iterator& begin, const CardDeck::iterator& end,
     const Card* skipCard=nullptr, bool failure_only_duplicate_symbols=false); 
bool checkCardDeck(CardDeck& deck);

CardDeck generateCardDeck(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics);


#ifdef DEBUG
void print(const Card& card);
void println(const Card& card);
#endif