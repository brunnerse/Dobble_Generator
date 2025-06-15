#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <map>

#include <assert.h>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>


namespace DobbleGenerator {

CardDeck generateIteratively(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics) 
{
    if (nSymbolsPerCard <= 1)
        return CardDeck(nSymbolsPerCard, Card(nSymbolsPerCard, 0));

    // Expected number of cards according to DorFuchs
    CardDeck deck; 
    deck.reserve(nSymbolsPerCard * (nSymbolsPerCard-1) + 1);

    std::vector<uint32_t> symbol_match_count(0, 0); 
    int32_t highest_symbol = -1;
    // Gives the number corresponding to the highest current symbol

    bool is_last_created_card_valid = true;

    while (is_last_created_card_valid && deck.size() < 10)
    { 
#if DEBUG
        printf("\n==========\nTrying to create card No. %zu...\n", deck.size()+1);
        printf("\nBase symbols match count:\n");
        for (SymbolId sym = 0; sym < symbol_match_count.size(); sym++)
            printf("%6x", sym);
        printf("\n");
        for (uint32_t sym_count : symbol_match_count)
            printf("%6u", sym_count); 
        printf("\n\n");
#endif

        Card card;
        card.reserve(nSymbolsPerCard);

        for (Card next_card_to_match : deck) 
        {
    #if DEBUG
                printf("\tTesting to match with card ");  println(next_card_to_match); 
    #endif

            uint32_t num_common_symbs = countCommonSymbols(card, next_card_to_match);
            if (num_common_symbs == 1) // Already correctly matching with next_card_to_match, continue with next card 
            {
    #if DEBUG
                printf("\t---- already matching\n");
    #endif
                continue;
            }
            else if (num_common_symbs > 1) {
                printf("\tERROR: New card already has more than one common symbol with another card, this must never happen\n");
                return deck;
            }

            // Sort symbols of card_to_match  by their symbol_match_count
            std::multimap<uint32_t, SymbolId> card_to_match_symbols;
            for (SymbolId id : next_card_to_match)
                card_to_match_symbols.insert(std::pair(symbol_match_count[id], id));

            assert(card_to_match_symbols.size() == nSymbolsPerCard);
            
            // Go through symbols in increasing match_count order and check if they can be used for the card
            bool found_matching_symbol = false;

            for (auto pair : card_to_match_symbols) 
            {
                // Test card with symbol
                SymbolId symbol = pair.second;
                card.push_back(symbol);
                if (checkCardAgainstDeck(card, deck.begin(), deck.end(), nullptr, true)) { 
                    found_matching_symbol = true;
                    symbol_match_count[symbol]++;
#if DEBUG
                    printf("\t---- matched with symbol %u\n", symbol);
#endif
                    break;
                }
                // If card does not work: Remove symbol from card again
                (void)card.pop_back();
            }

            if (!found_matching_symbol) {
#if DEBUG
                printf("\tCould not find a matching symbol that leads to a valid deck\n");
#endif
                is_last_created_card_valid = false;

            } else {
            }



/*
            uint32_t min_val = *std::min_element(base_symbol_count.begin(), base_symbol_count.end()); 

            uint32_t symbol_with_least_count = std::distance(base_symbol_count.begin(), min_iter);

            card.push_back(symbol_with_least_count);

            // Check card against entire deck (except the base card) 
*/
        }

        if (is_last_created_card_valid)
        {
            // Card is now matching with all previous cards; if card does not have all symbols yet, fill it up with new symbols
#if DEBUG
            if (card.size() < nSymbolsPerCard)
                printf("\t filling remaining symbols with new ones ...\n");
#endif
            // Fill remaining symbols with new ones
            while (card.size() < nSymbolsPerCard) {
                card.push_back(++highest_symbol); 
                symbol_match_count.push_back(0); // Create new entry for new symbol in symbol_match_count
            } 

            // Insert completed card into deck
            deck.push_back(card);
#if DEBUG
            printf("\n=> New Card (No. %zu):", deck.size()); println(deck.back()); 
#endif
    }
    } 

    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = (uint32_t)symbol_match_count.size(); 

    return deck;
}

}




