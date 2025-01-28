// HTMLParser.hpp
#pragma once
#include <vector>
#include "Token.hpp"
#include "DOMNode.hpp"

class HTMLParser {
public:
    HTMLParser(const std::vector<Token>& tokens);

    DOMNode* parse();

private:
    // For iteration
    const std::vector<Token>& m_tokens;
    size_t m_index = 0;

    DOMNode* m_document = nullptr;

    DOMNode* parseNodes();
    DOMNode* parseElement();

    bool isAtEnd() const;
    const Token& currentToken() const;
    void advance();
};
