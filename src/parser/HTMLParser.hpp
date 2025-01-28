#pragma once

#include "../dom/DOMDocument.hpp"
#include "HTMLLexer.hpp"

class HTMLParser
{
public:
    /**
     * @brief Parse an HTML string into a DOMDocument.
     */
    DOMDocument parse(const std::string &html);

private:
    // Utility to read next token, keep an index, etc.
    void advance();

    Token currentToken() const;

    void parseNodes(DOMElement *parent);

    void parseElement(DOMElement *parent);

    HTMLLexer *m_lexer = nullptr;
    std::vector<Token> m_tokens;
    size_t m_index = 0; // current token index
};
