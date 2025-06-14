#include <vector>
#include <iterator>
#include <numeric>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>

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
        std::iota(card.begin()+1, card.end(), highestSymbol+1);
        highestSymbol = card.back();
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



    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = highestSymbol + 1;

    return deck;
}



}

