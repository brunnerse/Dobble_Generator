#include <vector>
#include <iterator>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>



CardDeck generateCardDeck(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics) 
{
    CardDeck deck; 
    // Expected number of cards according to DorFuchs
    deck.reserve(nSymbolsPerCard * (nSymbolsPerCard-1) + 1);

    // Put base card into the deck and initialize it
    Card base(nSymbolsPerCard);
    for (uint32_t i = 0;  i < base.size(); i++) {
        base[i] = i; 
    }
    deck.push_back(base);

#if DEBUG
    printf("=> Base Card (No. 1):"); println(base);
#endif

    std::vector<bool> base_symbol_valid(nSymbolsPerCard, true); 
    uint32_t highest_sym_id = base.back();
    size_t last_base_symbol_idx = base.size()-1;

    while (deck.size() < 5) //TODO remove 
    { 
#if DEBUG
        printf("\n==========\nTrying to create card No. %zu...\n", deck.size()+1);
        printf("\nBase symbols valid:\n");
        for (uint32_t sym : base)
            printf("%6x", sym);
        printf("\n");
        for (bool sym_valid : base_symbol_valid)
            printf("%6c", sym_valid ? 'y' : 'x');
        printf("\n\n");
#endif


        // Check all symbols of base via round robin if base_symbol_valid is true
        size_t next_base_symbol_idx = last_base_symbol_idx;
        do {
            next_base_symbol_idx = (next_base_symbol_idx + 1) % base.size();
        } while (!base_symbol_valid[next_base_symbol_idx] && last_base_symbol_idx != next_base_symbol_idx);

        // Check if found a valid next base symbol; otherwise all base_symbol_valid must be false and we are done 
        if (!base_symbol_valid[next_base_symbol_idx])
            break;

        last_base_symbol_idx = next_base_symbol_idx;
        
#if DEBUG
        printf("Selected symbol idx base[%zx] = %2x as start\n", next_base_symbol_idx, base[next_base_symbol_idx]);
#endif

        
        Card card;
        card.reserve(nSymbolsPerCard);

        card.push_back(base[next_base_symbol_idx]); 
        uint32_t card_symbol_idx = 1; 

        for (CardDeck::iterator next_card_to_match = deck.begin()+1; *next_card_to_match != card; next_card_to_match++) 
        {
            uint32_t num_common_symbs = num_common_symbols(card, *next_card_to_match);
            if (num_common_symbs == 1) // Already matching with next_card_to_match, continue with next card 
                continue;
            else if (num_common_symbs > 1) {
                // TODO error handling
                break;
            }

            // No common symbols: test symbols from card 
            // Try symbol to match with next_card_to_match 
            size_t test_symbol_idx = 1;
            card.push_back((*next_card_to_match)[test_symbol_idx]);
#if DEBUG
            printf("[card %u]\t", next_card_to_match()-deck.begin());
//            printf("\t Testing to match with card ");  print(*next_card_to_match); printf("\tusing symbol %2x\n", card[test_symbol_idx]);
#endif

            // Do card against all previous cards (except the base card) 
            if (checkCardAgainstDeck(card, deck.begin()+1, deck.end())) {
                continue;
            } else {

            } 


            // If cannot create any more cards starting with current symbol, mark symbol as invalid and restart 
            if (false) {
            #if DEBUG
                printf("Exhausted all options with start symbol %2x, marking as invalid\n", 
                    base[next_base_symbol_idx]);
            #endif
                base_symbol_valid[last_base_symbol_idx] = false;
                deck.pop_back(); // Remove current card again 
                break;
            }
        }

#if DEBUG
        if (card_symbol_idx < card.size()-1)
            printf("\t Matched with all cards, filling remaining symbols with new ones %2x\n");
#endif
        // Fill remaining symbols with new ones
        while (card_symbol_idx < card.size()-1)
                card[card_symbol_idx++] = ++highest_sym_id; 

        // Insert completed card into deck
        deck.push_back(card);
#if DEBUG
        printf("=> New Card (No. %zu):", deck.size()); println(deck.back()); 
#endif

    } 

    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = highest_sym_id + 1;

    return deck;
}



