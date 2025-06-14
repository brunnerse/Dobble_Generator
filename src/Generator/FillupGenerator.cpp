#include <vector>
#include <iterator>
#include <numeric>

#include <assert.h>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>


#define FILL_SECOND_SYMBOL 1


namespace DobbleGenerator {


CardDeck generateByFillup(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics) 
{
    CardDeck deck; 
    uint32_t highestSymbol;

    if (nSymbolsPerCard < 1)
        return deck; 

    // Expected number of cards according to DorFuchs
    deck.reserve(nSymbolsPerCard * (nSymbolsPerCard-1) + 1);

    // Put base card into the deck and initialize it : 0 1 2 3 ... nSymbolsPerCard-1
    Card base(nSymbolsPerCard);
    std::iota(base.begin(), base.end(), 0);
    deck.push_back(base);
    highestSymbol = base.back();

#if DEBUG
    printf("=> Base Card (No. 1):"); println(base);
#endif
    // Each symbol appears on nSymbolsPerCard cards => Create (nSymbolsPerCard-1) cards per symbol, fill the first symbol
    for (SymbolId id : base) {
        for (unsigned i = 0; i < nSymbolsPerCard - 1; i++) {
            Card card(1);
            card[0] = id;
            deck.push_back(card);
        }
    }
#if DEBUG
            printf("\nCreated %u additional cards per symbol;\n"
                "Deck now contains %lu cards, symbols %u,...,%u each appear %u times in the deck\n",
                    nSymbolsPerCard-1, deck.size(), base.front(), base.back(), nSymbolsPerCard); 
#endif

    // Now: Fill up remaining numbers of all cards to match them with each other
#if DEBUG
            printf("\nFilling up symbols for cards 1-%u (Starting with symbol %u) with new symbols...\n", 
                    nSymbolsPerCard, base.front());
#endif
    for (unsigned i = 1; i < nSymbolsPerCard; i++) {
        Card& card = deck[i];
        while (card.size() < nSymbolsPerCard)
            card.push_back(++highestSymbol);
    }

#if DEBUG
    printf("We now have %lu cards and %u symbols\n",
            deck.size(), highestSymbol+1);
    // Print card deck with symbols
    printf("\nCard Deck:\n");
    for (uint32_t i = 0; i < deck.size(); i++) {
        printf("Card No. %2u:\t", i+1);
        Card &card = deck[i];
        for (SymbolId id : card) {
            printf("%4x", id);
        }
        printf("\n");
    }
#endif
    // We now created all cards and symbols: Can calculate the metrics 

    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = highestSymbol + 1;


    // Now we need to fill up the remaining cards until all numbers are correct
#if FILL_SECOND_SYMBOL
    if (nSymbolsPerCard < 2)
        return deck;
    // First: Fill second symbol for remaining cards with numbers of first card
    Card &second_card = deck[1];
    unsigned symbolIdx = 1;
    // Go through all possible permutations of the numbers of cards 
    for (auto card = deck.begin() + nSymbolsPerCard; card != deck.end(); card++)
    {
        card->push_back(second_card[symbolIdx]);
        symbolIdx = (symbolIdx < nSymbolsPerCard-1) ? symbolIdx+1 : 1;
    }

    uint32_t deck_start_idx = 2;
    uint32_t remaining_symbols_per_card = nSymbolsPerCard - 2;
    CardDeck::iterator next_card_to_fill = deck.begin() + nSymbolsPerCard;
    while (next_card_to_fill->size() == nSymbolsPerCard && next_card_to_fill != deck.end())
        next_card_to_fill++;
    std::vector<uint32_t> perm_idx(nSymbolsPerCard - 2, 1);
#else
    uint32_t deck_start_idx = 1;
    uint32_t remaining_symbols_per_card = nSymbolsPerCard - 1;
    CardDeck::iterator next_card_to_fill = deck.begin() + nSymbolsPerCard;
    std::vector<uint32_t> perm_idx(nSymbolsPerCard - 1, 1);
#endif

    while (next_card_to_fill != deck.end()) 
    {
        std::vector<SymbolId> symbol_permutation(remaining_symbols_per_card);
        for (unsigned i = 0; i < symbol_permutation.size(); i++)
            symbol_permutation[i] = deck[deck_start_idx + i][perm_idx[i]];

        // Check to which card we can insert the permutation
        for (auto iter_card = next_card_to_fill; iter_card != deck.end(); iter_card++) 
        {
            // If card is already full: Skip
            if (iter_card->size() == nSymbolsPerCard)
                continue;

            iter_card->insert(iter_card->end(), symbol_permutation.begin(), symbol_permutation.end());
            assert(iter_card->size() == nSymbolsPerCard);

            if (checkCardAgainstDeck(*iter_card, deck.begin(), deck.end(), nullptr, true)) {
                // Success: Filled out another card
#if DEBUG
                printf("\n=> Finished another Card: "); println(*iter_card); 
#endif
                // Go to next card that is not full yet 
                while (next_card_to_fill->size() == nSymbolsPerCard && next_card_to_fill != deck.end())
                    next_card_to_fill++;
                break;
            } else {
                // No Success: Remove symbols from card again
                iter_card->resize(iter_card->size() - symbol_permutation.size());
            }
        }

        // Select next permutation
        for (auto iter_idx = perm_idx.rbegin(); iter_idx != perm_idx.rend(); iter_idx++) {
            if (*iter_idx == nSymbolsPerCard-1) {
                *iter_idx = 1;
            } else {
                *iter_idx += 1;
                break;
            }
        }
    }
    return deck;
}



}

