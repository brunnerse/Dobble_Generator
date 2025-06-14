#pragma once
#include <stdint.h>
#include <vector>

#include "Dobble.h"


enum class GeneratorMethod {
    ITERATIVELY,
    FILLUP
};

namespace DobbleGenerator {
    CardDeck generateByFillup(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics);
    CardDeck generateIteratively(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics);
}

inline CardDeck generateCardDeck(uint32_t nSymbolsPerCard, CardDeckMetrics *out_metrics, GeneratorMethod method) {
    switch (method) {
        case GeneratorMethod::ITERATIVELY:
            return DobbleGenerator::generateIteratively(nSymbolsPerCard, out_metrics);
            break;
        default:
        case GeneratorMethod::FILLUP:
            return DobbleGenerator::generateByFillup(nSymbolsPerCard, out_metrics);
            break;
    }
}


