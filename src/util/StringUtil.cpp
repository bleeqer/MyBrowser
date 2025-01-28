#include <string>

auto trim = [](std::string &s) {
    // Trim front
    s.erase(s.begin(),
        std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        })
    );
    // Trim back
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(),
        s.end()
    );
};