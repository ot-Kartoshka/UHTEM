#pragma once
#include <string_view>

enum class Error {
    FileNotFound,
    EmptyFile,
    TextTooShort,
    InvalidArgument,
    NotEnoughExperiments
};

std::string_view Error_to_string(Error err);