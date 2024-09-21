#include <stdio.h>
#include <stdlib.h>

#include "Dobble.h"


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf(
            "Usage: %s <n>\n"
            "Generates a card deck with n symbols per card\n",
            argv[0]);
        return 0;
    }
    uint32_t nSymbolsPerCard = strtol(argv[1], NULL, 10);

    // TODO output file in args?
    // TODO input symbol file in args?


    CardDeckMetrics metrics;
    CardDeck deck = generateCardDeck(nSymbolsPerCard, &metrics);

    if (deck.empty()){
        printf("Failed to generate card deck with %u symbols per card.\n", 
            nSymbolsPerCard);
        return 1;
    }

    printf("Generated card deck with "
            "%u symbols per card, %u cards and %u symbols\n",
            metrics.Num_Symbols_per_Card, metrics.Num_Cards, metrics.Num_Symbols);

    printf("Checking correctness of card deck...\n");

    bool isCorrect = checkCardDeck(deck);
    if (!isCorrect) {
        printf("Failed: Generated card deck is not correct\n");
        return 1;
    }


    // Create symbols
    std::vector<Symbol> symbols(metrics.Num_Symbols);
    for (uint32_t i = 0; i < symbols.size(); i++) {
        symbols[i].id = i;
        symbols[i].str = "TODO"; 
    }

    // Print card deck with symbols
    printf("\nCard Deck:\n");
    for (uint32_t i = 0; i < deck.size(); i++) {
        printf("Card No. %2u:\t", i+1);
        for (SymbolId id : deck[i]) {
            printf("%12s[%02x]", symbols[id].str, symbols[id].id);
        }
        printf("\n");
    }
    

    return 0;
}