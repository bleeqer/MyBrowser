#pragma once
#include <string>

class ParserContext {
public:
    ParserContext(const std::string& html, size_t startPos);

    const std::string& getHtml() const;
    size_t getPos() const;
    void   setPos(size_t p);

    // Helpers
    char peek() const;
    char get();
    bool isAtEnd() const;
    bool startsWith(const std::string& str) const;

private:
    const std::string& m_html;
    size_t m_pos;
};
