#include <stdio.h>
#include <stdlib.h>

#include "Dobble.h"
#include "DobbleGenerator.h"


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf(
            "Usage: %s <n>\n"
            "Generates a card deck with n symbols per card\n",
            argv[0]);
        return 0;
    }
    uint32_t nSymbolsPerCard = (uint32_t)strtol(argv[1], NULL, 10);

    printf("Generating card deck with %u symbols per card...\n\n", nSymbolsPerCard);

    // TODO output file in args?
    // TODO input symbol file in args?


    CardDeckMetrics metrics;
    CardDeck deck = generateCardDeck(nSymbolsPerCard, &metrics, GeneratorMethod::FILLUP);

    if (deck.empty()){
        printf("Failed to generate card deck with %u symbols per card.\n", 
            nSymbolsPerCard);
        return 1;
    }

    printf("\n--------------------------\nGenerated card deck with "
            "%u symbols per card, %u cards and %u symbols\n",
            metrics.Num_Symbols_per_Card, metrics.Num_Cards, metrics.Num_Symbols);
            
    printf("\t(Expected for %u symbols per card: %u cards and %u symbols\n",
            nSymbolsPerCard, nSymbolsPerCard * (nSymbolsPerCard-1) + 1, 00);

    printf("Checking correctness of card deck...\n");

    bool isCorrect = validateCardDeck(deck);
    if (!isCorrect) {
        printf("Failed: Generated card deck is not correct\n");
        printCardDeckAnalysis(deck);
        return 1;
    } else {
        printf("Success: Generated card deck is correct\n");
    }


    // Create symbols
    std::vector<Symbol> symbols(metrics.Num_Symbols);
    for (uint32_t i = 0; i < symbols.size(); i++) {
        symbols[i].str = "TODO"; 
    }

    // Print card deck with symbols
    printf("\nCard Deck:\n");
    for (uint32_t i = 0; i < deck.size(); i++) {
        printf("Card No. %2u:\t", i+1);
        Card &card = deck[i];
        for (SymbolId id : card) {
//            printf("%12s[%01x]", symbols[id].str, id);
            printf("%4x", id);
        }
        printf("\n");
    }
    

    return 0;
}

