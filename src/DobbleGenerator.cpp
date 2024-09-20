#include <vector>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>


#ifdef DEBUG
inline void print_card(Card& card) {
    for (uint32_t symbol_id : card) {
        printf("%2x\t", symbol_id);
    }
}
#endif

CardDeck generateCardDeck(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics) 
{
    CardDeck deck(1); 
    // Expected number of cards according to DorFuchs
    deck.reserve(nSymbolsPerCard * (nSymbolsPerCard-1) + 1);

    Card& base = deck[0]; 
    base = Card(nSymbolsPerCard);
    for (uint32_t i = 0;  i < base.size(); i++) {
        base[i] = i; 
    }


#if DEBUG
    printf("Base Card: "); print_card(base); printf("\n"); 
#endif

    std::vector<bool> base_symbol_valid(nSymbolsPerCard, true); 
    uint32_t highest_sym_id = base.back();
    size_t last_base_symbol_idx = base.size()-1;

    while (true) {

        size_t next_base_symbol_idx = last_base_symbol_idx;
        do {
            next_base_symbol_idx = (next_base_symbol_idx + 1) % base.size();
            if (base_symbol_valid[next_base_symbol_idx] == true){
                break;
            }
        } while (!base_symbol_valid[next_base_symbol_idx] && last_base_symbol_idx);

        // Check if found a valid next base symbol; otherwise generation is exhausted, return
        if (!base_symbol_valid[next_base_symbol_idx])
            break;
        

        
        deck.push_back(Card(nSymbolsPerCard));
        Card& card = deck.back(); 

        card[0] = base[next_base_symbol_idx]; 
        last_base_symbol_idx = next_base_symbol_idx;



        bool all_cards_matching = false;
        // TODO start at second symbol
        for (uint32_t& symbol_id : card) {
            // If matching with all existing cards, fill up the remaining symbol_ids with new ones 
            if (all_cards_matching) {
                symbol_id = ++highest_sym_id; 
                continue;
            }


        }

    } 

    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = highest_sym_id + 1;

    return deck;
}



