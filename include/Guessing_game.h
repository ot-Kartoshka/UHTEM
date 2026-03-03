#pragma once
#include <cstddef>
#include <expected>
#include <span>
#include <cstdint>
#include "Entropy.h"
#include "Errors.h"

class GuessingGame {
public:
    static std::expected<GuessStats, Error> Run(
        std::span<const uint8_t> text,
        int n,
        std::size_t experiments = 50);

    static void PrintResults(const GuessStats& stats);
};