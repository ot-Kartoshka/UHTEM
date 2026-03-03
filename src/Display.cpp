#include "../include/Display.h"
#include "../include/Alphabet.h"
#include <algorithm>
#include <print>
#include <string>
#include <tuple>
#include <vector>

void Display::UnigramTable(const UnigramCounter& uc) {
    std::vector<std::pair<std::size_t, std::size_t>> pairs;
    pairs.reserve(ALPHABET_SIZE);
    for (std::size_t i = 0; i < ALPHABET_SIZE; ++i) {
        pairs.emplace_back(uc.Get(i), i);
    }
    std::ranges::sort(pairs, std::greater{});

    std::println("••••••••••••••••••••••••••••••••••••••••••");
    std::println("•  Символ  •  Частота  •  Відн. частота  •");
    std::println("••••••••••••••••••••••••••••••••••••••••••");
    for (auto& [cnt, idx] : pairs) {
        std::printf("\x95     %-5s\x95 %9zu \x95     %10.6f  \x95\n", idx_to_str(idx).c_str(), cnt, uc.Prob(idx));
    }
    std::println("••••••••••••••••••••••••••••••••••••••••••");
    std::println("    Всього: {} символів", uc.Total());
}

void Display::BigramTop(const BigramCounter& bc, std::size_t top_n) {
    using T3 = std::tuple<std::size_t, std::size_t, std::size_t>;
    std::vector<T3> v;
    v.reserve(ALPHABET_SIZE * ALPHABET_SIZE);

    for (std::size_t a = 0; a < ALPHABET_SIZE; ++a) {
        for (std::size_t b = 0; b < ALPHABET_SIZE; ++b) {
            if (bc.Get(a, b) > 0) v.emplace_back(bc.Get(a, b), a, b);
        }
    }
    std::ranges::sort(v, std::greater{});

    std::println("Топ-{} біграм:", top_n);
    std::println("•••••••••••••••••••••••••••••••••••••••••••");
    std::println("•  Біграма  •  Частота  •  Відн. частота  •");
    std::println("•••••••••••••••••••••••••••••••••••••••••••");

    std::size_t shown = 0;
    for (auto& [cnt, a, b] : v) {
        if (shown++ >= top_n) break;
        std::string sa = (a == 0) ? "_" : idx_to_str(a);
        std::string sb = (b == 0) ? "_" : idx_to_str(b);
        std::printf("\x95   %2s%-2s    \x95%9zu  \x95   %10.6f    \x95\n", sa.c_str(), sb.c_str(), cnt, bc.Prob(a, b));
    }
    std::println("•••••••••••••••••••••••••••••••••••••••••••");
    std::println("    Унікальних біграм: {}", v.size());
}

void Display::BigramMatrix(const BigramCounter& bc) {
    std::print("    ");
    for (std::size_t b = 0; b < ALPHABET_SIZE; ++b) {
        std::printf("%5s", (b == 0) ? "SPC" : idx_to_str(b).c_str());
    }
    std::println();

    for (std::size_t a = 0; a < ALPHABET_SIZE; ++a) {
        std::printf("%4s", (a == 0) ? "SPC" : idx_to_str(a).c_str());
        for (std::size_t b = 0; b < ALPHABET_SIZE; ++b) {
            std::size_t cnt = bc.Get(a, b);
            if (cnt == 0) std::print(" . ");
            else std::printf("%5zu", cnt);
        }
        std::println();
    }
}