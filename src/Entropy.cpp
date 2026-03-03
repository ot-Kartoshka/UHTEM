#include "../include/Entropy.h"
#include <cmath>

double EntropyCalc::Term(double p) noexcept {
    return (p > 0.0) ? -p * std::log2(p) : 0.0;
}

double EntropyCalc::H0(std::size_t m) noexcept {
    return std::log2(static_cast<double>(m));
}

double EntropyCalc::H1(const UnigramCounter& uc) noexcept {
    double h = 0.0;
    for (std::size_t i = 0; i < ALPHABET_SIZE; ++i) {
        h += Term(uc.Prob(i));
    }
    return h;
}

double EntropyCalc::H2(const BigramCounter& bc) noexcept {
    double h = 0.0;
    for (std::size_t a = 0; a < ALPHABET_SIZE; ++a) {
        for (std::size_t b = 0; b < ALPHABET_SIZE; ++b) {
            h += Term(bc.Prob(a, b));
        }
    }
    return h / 2.0;
}

double EntropyCalc::Redundancy(double h_inf, double h0) noexcept {
    return (h0 > 0.0) ? 1.0 - h_inf / h0 : 0.0;
}


HnBounds EntropyCalc::HnBounds_(const GuessStats& stats) noexcept {
    const std::size_t m = stats.q.size();
    if (m == 0) return {};

    double upper = 0.0;
    for (std::size_t i = 0; i < m; ++i) {
        upper += Term(stats.q[i]);
    }

    double lower = 0.0;
    for (std::size_t i = 1; i < m; ++i) {
        double diff = stats.q[i - 1] - stats.q[i];
        if (i > 1 && diff > 0.0) {
            lower += static_cast<double>(i) * diff * std::log2(static_cast<double>(i));
        }
    }
    lower += static_cast<double>(m) * stats.q[m - 1] * std::log2(static_cast<double>(m));

    return { lower, upper };
}