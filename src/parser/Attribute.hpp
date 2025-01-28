#pragma once
#include <string>

struct Attribute {
    std::string name;
    std::string value;
    bool hasValue = false;
};
