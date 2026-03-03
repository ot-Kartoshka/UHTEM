#include "../include/Text_filter.h"
#include "../include/Alphabet.h"
#include <fstream>
#include <print>


std::expected<std::vector<uint8_t>, Error> TextFilter::Load(const std::filesystem::path& path, bool include_spaces) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::unexpected(Error::FileNotFound);
    }

    std::string raw((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (raw.empty()) {
        return std::unexpected(Error::EmptyFile);
    }

    std::vector<uint8_t> result;
    result.reserve(raw.size());

    bool prev_space = true; 

    for (unsigned char byte : raw) {
        int8_t idx = get_char_to_idx()[byte];

        if (idx <= 0) {
            if (include_spaces && !prev_space) {
                result.push_back(0);
                prev_space = true;
            }
        }
        else {
            result.push_back(static_cast<uint8_t>(idx));
            prev_space = false;
        }
    }

    if (!result.empty() && result.back() == 0) {
        result.pop_back();
    }

    if (result.size() < 2) {
        return std::unexpected(Error::TextTooShort);
    }

    return result;
}