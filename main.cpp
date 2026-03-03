#include <charconv>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>
#include <vector>

#include "./include/Alphabet.h"
#include "./include/Display.h"
#include "./include/Entropy.h"
#include "./include/Errors.h"
#include "./include/Frequency.h"
#include "./include/Guessing_game.h"
#include "./include/Text_filter.h"


static std::vector<uint8_t> text;
static std::vector<uint8_t> text_ns;


static bool parse_int(std::string_view s, int& out) noexcept {
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), out);
    return ec == std::errc{} && ptr == s.data() + s.size();
}

static bool parse_size(std::string_view s, std::size_t& out) noexcept {
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), out);
    return ec == std::errc{} && ptr == s.data() + s.size();
}


static void do_load() {
    std::print("Шлях до файлу (Enter = TEXT): ");
    std::cout.flush();
    std::string path;
    std::getline(std::cin, path);
    if (path.empty()) path = "TEXT";

    auto r1 = TextFilter::Load(path, true);
    if (!r1) { 
        std::println(stderr,"    Error: {}", Error_to_string(r1.error())); 
        return;
    }
    text = std::move(*r1);

    auto r2 = TextFilter::Load(path, false);
    if (!r2) {
        std::println(stderr, "    Варіант без пробілів: {}", Error_to_string(r2.error()));
    }
    else {
        text_ns = std::move(*r2);
    }

    std::println("    Завантажено: {} симв. (з пробілами), {} (без).", text.size(), text_ns.size());
}


static void print_entropies(const std::vector<uint8_t>& text, std::size_t m) {
    double h0 = EntropyCalc::H0(m);
    std::println("    H0 = log2({}) = {:.6f} біт/символ", m, h0);

    UnigramCounter uc;
    uc.Count(text);
    double h1 = EntropyCalc::H1(uc);
    std::println("    H1 = {:.6f}  •  R = {:.4f} ({:.2f}%)", h1, EntropyCalc::Redundancy(h1, h0), EntropyCalc::Redundancy(h1, h0) * 100.0);

    BigramCounter bc_ov, bc_nov;
    bc_ov.Count(text, true);
    bc_nov.Count(text, false);
    double h2_ov = EntropyCalc::H2(bc_ov);
    double h2_nov = EntropyCalc::H2(bc_nov);
    std::println("    H2 (перетинні)   = {:.6f}  •  R = {:.4f} ({:.2f}%)", h2_ov, EntropyCalc::Redundancy(h2_ov, h0), EntropyCalc::Redundancy(h2_ov, h0) * 100.0);
    std::println("    H2 (неперетинні) = {:.6f}  •  R = {:.4f} ({:.2f}%)", h2_nov, EntropyCalc::Redundancy(h2_nov, h0), EntropyCalc::Redundancy(h2_nov, h0) * 100.0);

    std::println("\nТаблиця частот символів:");
    Display::UnigramTable(uc);

    std::println("\nТоп-30 біграм (перетинні):");
    Display::BigramTop(bc_ov, 30);
}

static void do_with_spaces() {
    if (text.empty()) { std::println("    Спочатку завантажте текст (0)."); return; }
    std::println("\n••••••••••• Текст з пробілами ({} символів) ••••••••••", text.size());
    print_entropies(text, ALPHABET_SIZE);
}


static void do_without_spaces() {
    if (text_ns.empty()) { std::println("    Текст без пробілів недоступний."); return; }
    std::println("\n•••••••••• Текст БЕЗ пробілів ({} символів) ••••••••••", text_ns.size());
    print_entropies(text_ns, ALPHABET_SIZE - 1);
}


static void do_matrix() {
    if (text.empty()) { 
        std::println("  Спочатку завантажте текст (0)."); 
        return; 
    }
    std::print("[1] з пробілами  [2] без пробілів: ");
    std::cout.flush();
    std::string c;
    std::getline(std::cin, c);

    const auto& selected_text = (c == "2" && !text_ns.empty()) ? text_ns : text;
    BigramCounter bc;
    bc.Count(selected_text, true);
    std::println("\nМатриця біграм (рядок = перша буква, стовпець = друга):");
    Display::BigramMatrix(bc);
}


static void do_guessing() {
    if (text.empty()) { 
        std::println("    Спочатку завантажте текст (0)."); 
        return; 
    }

    std::print("Введіть n (10 / 20 / 30 / ...): ");
    std::cout.flush();
    std::string line;
    std::getline(std::cin, line);

    int n = 0;
    if (!parse_int(line, n) || n < 1 || n > 50) {
        std::println("    {}", Error_to_string(Error::InvalidArgument));
        return;
    }

    std::print("Кількість дослідів (Enter = 50): ");
    std::cout.flush();
    std::getline(std::cin, line);

    std::size_t experiments = 50;
    if (!line.empty() && !parse_size(line, experiments)) {
        std::println("    Некоректне значення, використовую 50.");
    }
    if (experiments < 50) {
        std::println("    {}", Error_to_string(Error::NotEnoughExperiments));
    }

    auto result = GuessingGame::Run(text, n, experiments);
    if (!result) { 
        std::println("    {}", Error_to_string(result.error())); 
        return; 
    }

    GuessingGame::PrintResults(*result);

    auto [lo, hi] = EntropyCalc::HnBounds_(*result);
    double h0 = EntropyCalc::H0(ALPHABET_SIZE);
    std::println("        R (за верхньою межею): {:.4f} ({:.2f}%)", EntropyCalc::Redundancy(hi, h0), EntropyCalc::Redundancy(hi, h0) * 100.0);
}


static void do_summary() {
    if (text.empty()) {
        std::println("    Спочатку завантажте текст (0)."); 
        return; 
    }

    double h0 = EntropyCalc::H0(ALPHABET_SIZE);
    double h0_ns = EntropyCalc::H0(ALPHABET_SIZE - 1);

    UnigramCounter uc1, uc2;
    uc1.Count(text); uc2.Count(text_ns);
    BigramCounter bc1, bc2;
    bc1.Count(text, true); bc2.Count(text_ns, true);

    double h1 = EntropyCalc::H1(uc1), h1_ns = EntropyCalc::H1(uc2);
    double h2 = EntropyCalc::H2(bc1), h2_ns = EntropyCalc::H2(bc2);

    std::println("\n•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••");
    std::println("•         Модель         •  Ентропія  •    H_inf    •    R    •");
    std::println("•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••");
    std::println("• H0 (з пробілами, m=32) • {:>10.6f} •      -     •     -    •", h0);
    std::println("• H0 (без пробілів,m=31) • {:>10.6f} •      -     •     -    •", h0_ns);
    std::println("• H1 (з пробілами)       • {:>10.6f} • {:>9.6f}  • {:>8.4f} •", h1, h1, EntropyCalc::Redundancy(h1, h0));
    std::println("• H1 (без пробілів)      • {:>10.6f} • {:>9.6f}  • {:>8.4f} •", h1_ns, h1_ns, EntropyCalc::Redundancy(h1_ns, h0_ns));
    std::println("• H2 (з пробілами)       • {:>10.6f} • {:>9.6f}  • {:>8.4f} •", h2, h2, EntropyCalc::Redundancy(h2, h0));
    std::println("• H2 (без пробілів)      • {:>10.6f} • {:>9.6f}  • {:>8.4f} •", h2_ns, h2_ns, EntropyCalc::Redundancy(h2_ns, h0_ns));
    std::println("•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••");
}


#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string choice;
    while (true) {
        std::println("• • • • • • • • • • • • • • • • • •");
        std::println("•  0. Завантажити файл            •");
        std::println("•  1. H0, H1, H2  (з пробілами)   •");
        std::println("•  2. H0, H1, H2  (без пробілів)  •");
        std::println("•  3. Матриця біграм 32x32        •");
        std::println("•  4. Гра-вгадування H^(n)        •");
        std::println("•  5. Зведена таблиця             •");
        std::println("•  q. Вихід                       •");
        std::println("• • • • • • • • • • • • • • • • • •");
        std::print("Вибір: ");
        std::cout.flush();

        if (!std::getline(std::cin, choice) || choice == "q" || choice == "Q") {
            std::println("BYE BYE!");
            break;
        }
        if (choice.empty()) continue;

        switch (choice[0]) {
        case '0': do_load();           break;
        case '1': do_with_spaces();    break;
        case '2': do_without_spaces(); break;
        case '3': do_matrix();         break;
        case '4': do_guessing();       break;
        case '5': do_summary();        break;
        default:  std::println("  Невідома команда."); break;
        }
        std::println();
    }
    return 0;
}