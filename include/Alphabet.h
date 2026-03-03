#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <string>

constexpr std::size_t ALPHABET_SIZE = 32;

std::array<int8_t, 256> make_char_to_idx();

std::array<uint8_t, ALPHABET_SIZE> make_idx_to_char();

const std::array<int8_t, 256>& get_char_to_idx();
const std::array<uint8_t, ALPHABET_SIZE>& get_idx_to_char();

std::string idx_to_str(std::size_t idx);