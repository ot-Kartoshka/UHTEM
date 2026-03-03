#pragma once
#include <expected>
#include <filesystem>
#include <vector>
#include <cstdint>
#include "Errors.h"


class TextFilter {
public:
    static std::expected<std::vector<uint8_t>, Error> Load(
        const std::filesystem::path& path,
        bool include_spaces = true);
};