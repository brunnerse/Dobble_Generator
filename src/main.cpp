#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <fstream>


#include "Dobble.h"
#include "DobbleGenerator.h"


int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "-h") == 0) {
        printf(
            "Usage: %s <n> [-s symbolfile] [-o file]\n"
            "Generates a card deck with n symbols per card\n",
            argv[0]);
        return 0;
    }
    uint32_t nSymbolsPerCard = (uint32_t)strtol(argv[1], NULL, 10);

    std::string symbol_file("");
    std::string output_file("");
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0&& i+1 < argc)
            output_file = argv[++i]; 
        else if (strcmp(argv[i], "-s") == 0&& i+1 < argc)
            symbol_file = argv[++i]; 
    }

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
            nSymbolsPerCard, nSymbolsPerCard * (nSymbolsPerCard-1) + 1, nSymbolsPerCard * (nSymbolsPerCard-1)+1);

    printf("Checking correctness of card deck...\n");

    bool isCorrect = validateCardDeck(deck);
    if (!isCorrect) {
        printf("Failed: Generated card deck is not correct\n\n");
        printCardDeckAnalysis(deck);
        return 1;
    } else {
        printf("Success: Generated card deck is correct\n\n");
    }


    // Create symbols from symbols file
    std::vector<Symbol> symbols(0);
    symbols.reserve(metrics.Num_Symbols);
    size_t longest_symbol_len = 0;

    if (!symbol_file.empty()) {
        printf("Reading symbols from file '%s'\n", symbol_file.c_str());
        std::ifstream rfile(symbol_file);
        if (rfile.is_open()) {
            std::string sym_name;
            char c;
            while (!rfile.eof()) {
                rfile.get(c);
                if (std::isalpha(c))
                    sym_name.push_back(c);
                else 
                    if (!sym_name.empty()) {
                        symbols.push_back(std::string(sym_name));
                        longest_symbol_len = std::max(longest_symbol_len, sym_name.size());
                        sym_name.clear();
                    }
            }

            rfile.close();
        } else {
            printf("Error: Cannot read file\n");
        }
    }

    /*
    while (symbols.size() < metrics.Num_Symbols)
        symbols.push_back("");
    }
    */
        
    // Print card deck with symbols
    printf("\nCard Deck:\n");
    for (uint32_t i = 0; i < deck.size(); i++) {
        printf("Card No. %2u:\t", i+1);
        Card &card = deck[i];
        for (SymbolId id : card) {
                printf("%*s [%02x]", 
                    (int)(longest_symbol_len + 4), (id < symbols.size() ? symbols[id].c_str() : ""), id);
        }
        printf("\n");
    }

    if (!output_file.empty()) {
        std::ofstream file(output_file);

        if (file.is_open()) {
            for (uint32_t i = 0; i < deck.size(); i++) {
                file << "Card No. " << i+1 << ":   ";
                Card &card = deck[i];
                for (SymbolId id : card) {

                    if (id < symbols.size())
                        file << symbols[id] << "\t"; 
                    else
                        file << "[" << id << "]\t";
                }
                file << std::endl; 
            }

            file.close();
        }


    }
    

    return 0;
}

