#pragma once
#include <cstddef>
#include <vector>
#include "Frequency.h"


struct GuessStats {
    std::vector<double> q;
    std::size_t total_exp = 0;
    int n = 0;
};

struct HnBounds {
    double lower = 0.0;
    double upper = 0.0;
};


class EntropyCalc {
public:
    static double H0(std::size_t m = ALPHABET_SIZE) noexcept;

    static double H1(const UnigramCounter& uc) noexcept;

    static double H2(const BigramCounter& bc) noexcept;

    static double Redundancy(double h_inf, double h0) noexcept;

    static HnBounds HnBounds_(const GuessStats& stats) noexcept;

private:
    static double Term(double p) noexcept;
};