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

bool has_no_duplicate_symbols(Card& c) 
{
    for (auto iter = c.begin(); iter != c.end(); iter++) {
        for (auto iter2 = iter+1; iter2 != c.end(); iter2++) {
            if (*iter == *iter2) {
#if DEBUG
                printf("Failed during check: Card has duplicate symbol with id %2x. Card is", *iter); 
                println(c);
#endif
                return false;
            }
        }
    }
    return true;
}

bool has_exactly_one_common_symbol(Card& c1, Card& c2) 
{
    bool success = false;
    for (SymbolId sym : c1) {
        for (SymbolId sym2 : c2) {
            if (sym == sym2) {
                if (!success) {
                    success = true;
                } else {
                    printf("Failed during check: Cards do have more than one common symbol\nThe cards are:");
                    print(c1); printf(","); println(c2);
                    return false;
                }
            }
        }
    }

#if DEBUG
    if (!success) {
        printf("Failed during check: Cards do not have a common symbol\nThe cards are:");
        print(c1); printf(","); println(c2);
    }
#endif

    return success;
}


bool checkCardDeck(CardDeck& deck)
{
    for (auto iter = deck.rbegin(); iter != deck.rend(); iter++)
    {
        Card& card = *iter;

        if (!has_no_duplicate_symbols(card))
            return false;


#if EXHAUSTIVE_CHECK
        // Check all elements
        for (auto iter2 = deck.rbegin(); iter2 != deck.rend(); iter2++)
#else
        // Only check elements lower than current one, as all above have already been checked
        for (auto iter2 = iter+1; iter2 != deck.rend(); iter2++)
#endif
        {
            Card& card_to_compare = *iter2;
#if EXHAUSTIVE_CHECK
            // Do not compare card with itself
            if (card_to_compare == card)
                continue;
#endif
            if (!has_exactly_one_common_symbol(card, card_to_compare)) {
                return false;
            }
        }
    }
    return true;
}