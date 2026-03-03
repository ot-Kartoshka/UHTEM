#include "../include/Guessing_game.h"
#include "../include/Alphabet.h"
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

std::expected<GuessStats, Error> GuessingGame::Run(std::span<const uint8_t> text, int n, std::size_t experiments) {
    if (n < 1 || n > 50) {
        std::println(stderr, "GuessingGame Error: {}", Error_to_string(Error::InvalidArgument));
        return std::unexpected(Error::InvalidArgument);
    }

    const std::size_t ctx_len = static_cast<std::size_t>(n) - 1;

    if (text.size() < static_cast<std::size_t>(n) + 1) {
        std::println(stderr, "GuessingGame Error: {}", Error_to_string(Error::TextTooShort));
        return std::unexpected(Error::TextTooShort);
    }

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<std::size_t> dist(0, text.size() - n - 1);

    std::vector<std::size_t> attempt_hist(ALPHABET_SIZE, 0);
    std::size_t done = 0;

    std::println("•••••••••••••••••••••••••••••••••••••••••");
    std::println("  Гра-вгадування: оцінка H^({})", n);
    std::println("  Контекст: {} симв.  •  Дослідів: {}", ctx_len, experiments);
    std::println("  Вводьте одну букву та натискайте Enter.");
    std::println("•••••••••••••••••••••••••••••••••••••••••");

    while (done < experiments) {
        std::size_t pos = dist(rng);
        uint8_t correct = text[pos + ctx_len];

        printf("\n[%zu/%zu] ...", done + 1, experiments);
        for (std::size_t k = 0; k < ctx_len; ++k) {
            printf("%c", static_cast<char>(get_idx_to_char()[text[pos + k]]));
        }
        printf("...?\n");

        std::size_t attempt = 0;
        bool guessed = false;

        while (attempt < ALPHABET_SIZE) {
            std::print("  Спроба {}: ", attempt + 1);
            std::cout.flush();

            std::string line;
            if (!std::getline(std::cin, line) || line.empty()) {
                std::println("    (порожній ввід, спробуйте ще)");
                continue;
            }

            unsigned char byte = static_cast<unsigned char>(line[0]);
            int8_t idx = get_char_to_idx()[byte];

            if (idx < 0) {
                std::println("    (символ не в алфавіті, спробуйте ще)");
                continue;
            }

            if (static_cast<uint8_t>(idx) == correct) {
                std::println("  Правильно! (спроба {})", attempt + 1);
                ++attempt_hist[attempt];
                guessed = true;
                break;
            }
            std::println("    Невірно.");
            ++attempt;
        }

        if (!guessed) {
            printf("    Всі спроби вичерпано. Відповідь: %c\n", static_cast<char>(get_idx_to_char()[correct]));
            ++attempt_hist[ALPHABET_SIZE - 1];
        }
        ++done;
    }

    GuessStats stats;
    stats.n = n;
    stats.total_exp = done;
    stats.q.resize(ALPHABET_SIZE, 0.0);
    for (std::size_t i = 0; i < ALPHABET_SIZE; ++i) {
        stats.q[i] = static_cast<double>(attempt_hist[i]) / static_cast<double>(done);
    }
    return stats;
}

void GuessingGame::PrintResults(const GuessStats& stats) {
    std::println("\n••••• Результати (n={}, дослідів={}) •••••", stats.n, stats.total_exp);
    for (std::size_t i = 0; i < stats.q.size(); ++i)
        if (stats.q[i] > 0.0)
            std::println("    q_{} = {:.4f}", i + 1, stats.q[i]);

    auto [lo, hi] = EntropyCalc::HnBounds_(stats);
    std::println("\n    H^({}):", stats.n);
    std::println("        Нижня межа:  {:.4f} біт/символ", lo);
    std::println("        Верхня межа: {:.4f} біт/символ", hi);
}