#include "../include/Frequency.h"


void UnigramCounter::Count(std::span<const uint8_t> text) {
    counts_.fill(0);
    total_ = 0;
    for (uint8_t ch : text) {
        if (ch < ALPHABET_SIZE) {
            ++counts_[ch]; ++total_;
        }
    }
}

double UnigramCounter::Prob(std::size_t i) const noexcept {
    return (total_ > 0) ? static_cast<double>(counts_[i]) / static_cast<double>(total_) : 0.0;
}


void BigramCounter::Count(std::span<const uint8_t> text, bool overlap) {
    for (auto& row : counts_) row.fill(0);
    total_ = 0;

    if (text.size() < 2) return;

    const std::size_t step = overlap ? 1u : 2u;
    const std::size_t limit = text.size() - 1;

    for (std::size_t i = 0; i < limit; i += step) {
        uint8_t a = text[i], b = text[i + 1];
        if (a < ALPHABET_SIZE && b < ALPHABET_SIZE) {
            ++counts_[a][b];
            ++total_;
        }
    }
}

double BigramCounter::Prob(std::size_t a, std::size_t b) const noexcept {
    return (total_ > 0) ? static_cast<double>(counts_[a][b]) / static_cast<double>(total_) : 0.0;
}