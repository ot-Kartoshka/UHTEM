#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include "Alphabet.h"


class UnigramCounter {
public:
    void Count(std::span<const uint8_t> text);

    std::size_t Total() const noexcept { return total_; }
    std::size_t Get(std::size_t i) const noexcept { return counts_[i]; }
    double Prob(std::size_t i) const noexcept;

    const std::array<std::size_t, ALPHABET_SIZE>& Counts() const noexcept { return counts_; }

private:
    std::array<std::size_t, ALPHABET_SIZE> counts_{};
    std::size_t total_ = 0;
};


class BigramCounter {
public:
    void Count(std::span<const uint8_t> text, bool overlap);

    std::size_t Total() const noexcept { return total_; }
    std::size_t Get(std::size_t a, std::size_t b) const noexcept { return counts_[a][b]; }
    double Prob(std::size_t a, std::size_t b) const noexcept;

    const std::array<std::array<std::size_t, ALPHABET_SIZE>, ALPHABET_SIZE>& Counts() const noexcept { return counts_; }

private:
    std::array<std::array<std::size_t, ALPHABET_SIZE>, ALPHABET_SIZE> counts_{};
    std::size_t total_ = 0;
};