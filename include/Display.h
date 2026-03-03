#pragma once
#include <cstddef>
#include "Frequency.h"

class Display {
public:
    static void UnigramTable(const UnigramCounter& uc);

    static void BigramTop(const BigramCounter& bc, std::size_t top_n = 20);

    static void BigramMatrix(const BigramCounter& bc);
};