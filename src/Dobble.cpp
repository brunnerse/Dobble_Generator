#include "Dobble.h"

#define EXHAUSTIVE_CHECK 1

#ifdef DEBUG
void print(const Card& card) {
    for (uint32_t symbol_id : card) {
        printf("%4x", symbol_id);
    }
}
void println(const Card& card) {
    print(card);
    printf("\n");
}
#endif

bool has_no_duplicate_symbols(const Card& c) 
{
    for (auto iter = c.begin(); iter != c.end(); iter++) {
        for (auto iter2 = iter+1; iter2 != c.end(); iter2++) {
            if (*iter == *iter2) {
#if DEBUG
//                printf("Failed during check: Card has duplicate symbol with id %2x. Card is", *iter); 
//                println(c);
#endif
                return false;
            }
        }
    }
    return true;
}

uint32_t countCommonSymbols(const Card& c1, const Card& c2, bool fast) 
{
    uint32_t common = 0;
    for (SymbolId sym : c1) {
        for (SymbolId sym2 : c2) {
            if (sym == sym2) {
                common++;
                if (fast && common > 1) {
#if DEBUG
//                    printf("Failed during check: Cards do have more than one common symbol\nThe cards are:");
//                    print(c1); printf(" and "); println(c2);
#endif
                    return common;
                }
            }
        }
    }

#if DEBUG
    if (common == 0) {
//     printf("Failed during check: Cards do not have a common symbol\nThe cards are:");
//     print(c1); printf(","); println(c2);
 } else {
//   printf("Passed check: Cards do have exactly one common symbol\nThe cards are:");
//    print(c1); printf(" and "); println(c2);
 }
#endif

    return common;
}


bool checkCardAgainstDeck(const Card& card, const CardDeck::iterator& begin, const CardDeck::iterator& end, const Card* skipCard, bool failure_only_duplicate_symbols) 
{
    for (auto iter = begin; iter != end; iter++) {
        Card& card_to_compare = *iter;
        // Do not compare card with itself (TODO: check not always necessary)
        if (&card_to_compare == &card || &card_to_compare == skipCard)
            continue;
        uint32_t num_common = countCommonSymbols(card, card_to_compare);
        if (failure_only_duplicate_symbols) {
            if (num_common > 1)
                return false;
        } else {
            if (num_common != 1)
                return false;
        }
    }        
    return true;
}

bool checkCardDeck(CardDeck& deck)
{
    for (auto iter = deck.begin(); iter != deck.end(); iter++)
    {
        const Card& card = *iter;

        if (!has_no_duplicate_symbols(card))
            return false;

#if EXHAUSTIVE_CHECK
        // Check all elements
        if (!checkCardAgainstDeck(card, deck.begin(), deck.end()))
            return false;
#else
        // Only check elements higher than the current one, as all below have already been checked
        if (!checkCardAgainstDeck(card, iter+1, deck.end()))
            return false;
#endif
    }
    return true;
}