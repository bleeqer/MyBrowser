#pragma once

#include "TokenType.hpp"
#include <string>
#include <vector>
#include <utility>

struct Token {
    TokenType type;
    std::string value;

    std::vector<std::pair<std::string, std::string>> attributes;
};
