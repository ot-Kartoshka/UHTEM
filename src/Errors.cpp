#include "../include/Errors.h"

std::string_view Error_to_string(Error err) {
    switch (err) {
    case Error::FileNotFound:           return "Файл не знайдено за вказаним шляхом.";
    case Error::EmptyFile:              return "Файл порожній.";
    case Error::TextTooShort:           return "Текст занадто короткий для аналізу.";
    case Error::InvalidArgument:        return "Некоректний аргумент.";
    case Error::NotEnoughExperiments:   return "Рекомендується не менше 50 експериментів.";
    default:                            return "Невідома помилка.";
    }
}

