#include <vector>
#include <iterator>
#include <numeric>

#include <assert.h>

#include "Dobble.h"


#ifdef DEBUG
#include <stdio.h>
#endif
#include <stddef.h>

namespace DobbleGenerator {


CardDeck generateByCheckingAllPermutations(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics) 
{
    CardDeck deck; 

    if (nSymbolsPerCard < 1)
        return deck; 

    // Expected number of cards according to DorFuchs
    uint32_t nSymbolsTotal = nSymbolsPerCard * (nSymbolsPerCard-1) + 1;
    deck.reserve(nSymbolsTotal);

    // Stupidly try all permutations
    std::vector<uint32_t> perm_idx(nSymbolsTotal);


    bool checked_all_permutations = false;
    Card test_card(nSymbolsPerCard);

    while (!checked_all_permutations)
    {
        for (unsigned i = 0; i < test_card.size(); i++)
            test_card[i] = perm_idx[i];

        if (has_no_duplicate_symbols(test_card)) {
            if (checkCardAgainstDeck(test_card, deck.begin(), deck.end(), nullptr, true)) {
                // Success: Filled out another card
#if DEBUG
                printf("\n=> Finished another Card (No. %lu): ", deck.size()); println(test_card); 
#endif
                deck.push_back(Card(test_card));
            } 
        }

        // Select next permutation
        for (auto iter_idx = perm_idx.rbegin(); iter_idx != perm_idx.rend(); iter_idx++) {
            if (*iter_idx == nSymbolsTotal - 1) {
                *iter_idx = 0;
                if (iter_idx + 1 == perm_idx.rend())
                    checked_all_permutations = true;
            } else {
                *iter_idx += 1;
                break;
            }
        }
    }



    out_metrics->Num_Cards = (uint32_t)deck.size();
    out_metrics->Num_Symbols_per_Card = nSymbolsPerCard;
    out_metrics->Num_Symbols = nSymbolsTotal; 

    return deck;
}



}

