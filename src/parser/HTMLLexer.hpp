#pragma once

#include <string>
#include <vector>

/**
 * Basic token types for a simplified HTML lexer.
 */
enum class TokenType
{
    Text,
    OpenTag,
    CloseTag,
    SelfClosingTag,
    Comment,
    Doctype,
    EndOfFile,
    Skip
};

struct Token
{
    TokenType type;
    std::string value; // e.g. tag name, comment text, doctype text, etc.
};

/**
 * @brief A minimal HTML lexer that converts a string into a sequence of tokens.
 */
class HTMLLexer
{
public:
    explicit HTMLLexer(const std::string &html);

    std::vector<Token> tokenize();

private:
    Token parseText();

    Token parseTag();

    Token parseComment();

    Token parseDoctype();

    void skipWhitespace();

    bool detectClosingTag();

    std::string readTagName();

    void skipMalformedTag();

    bool detectSelfClosing(const std::string &tagName);

    // Helpers to inspect the input
    char peek() const; // Look at current character
    char peekNext() const; // Look at next character, if needed
    char get(); // Consume one character and advance
    bool isAtEnd() const; // True if we've reached the end
    bool startsWith(const std::string &str) const; // Check if upcoming chars match str

private:
    const std::string &m_html;
    size_t m_pos{0}; // Current index in the input
    bool m_sawSlashBeforeClose{false};
};
