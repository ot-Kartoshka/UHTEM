#include "../include/Alphabet.h"

std::array<int8_t, 256> make_char_to_idx() {
    std::array<int8_t, 256> t{};
    for (auto& v : t) v = -1;
    t[0x20] = 0; // spc

    for (int i = 0; i < 16; ++i) {
        t[0xE0 + i] = static_cast<int8_t>(1 + i);  // а–п 
        t[0xC0 + i] = static_cast<int8_t>(1 + i);  // А–П 
    }
    for (int i = 0; i < 10; ++i) {
        t[0xF0 + i] = static_cast<int8_t>(17 + i); // р–щ 
        t[0xD0 + i] = static_cast<int8_t>(17 + i); // Р–Щ 
    }
    t[0xFB] = 27; t[0xDB] = 27; // ы Ы
    t[0xFC] = 28; t[0xDC] = 28; // ь Ь
    t[0xFA] = 28; t[0xDA] = 28; // ъ Ъ -> ь
    t[0xFD] = 29; t[0xDD] = 29; // э Э
    t[0xFE] = 30; t[0xDE] = 30; // ю Ю
    t[0xFF] = 31; t[0xDF] = 31; // я Я
    t[0xB8] = 6;  t[0xA8] = 6;  // ё Ё -> е
    return t;
}

std::array<uint8_t, ALPHABET_SIZE> make_idx_to_char() {
    std::array<uint8_t, ALPHABET_SIZE> t{};
    t[0] = 0x20;
    for (int i = 0; i < 16; ++i) t[1 + i] = static_cast<uint8_t>(0xE0 + i);
    for (int i = 0; i < 10; ++i) t[17 + i] = static_cast<uint8_t>(0xF0 + i);
    t[27] = 0xFB; t[28] = 0xFC; t[29] = 0xFD; t[30] = 0xFE; t[31] = 0xFF;
    return t;
}

const std::array<int8_t, 256>& get_char_to_idx() {
    static const auto t = make_char_to_idx();
    return t;
}

const std::array<uint8_t, ALPHABET_SIZE>& get_idx_to_char() {
    static const auto t = make_idx_to_char();
    return t;
}

std::string idx_to_str(std::size_t idx) {
    if (idx == 0) return " ";
    if (idx >= ALPHABET_SIZE) return "?";
    return std::string(1, static_cast<char>(get_idx_to_char()[idx]));
}