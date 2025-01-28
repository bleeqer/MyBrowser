#include "HTMLParser.hpp"
#include <iostream> // for logging

HTMLParser::HTMLParser(const std::vector<Token>& tokens)
    : m_tokens(tokens)
{
    m_document = new DOMNode("document");
}

DOMNode* HTMLParser::parse()
{
    while (!isAtEnd()) {
        const Token& t = currentToken();
        if (t.type == TokenType::Doctype) {
            advance();
        }
        else if (t.type == TokenType::Comment) {
            advance();
        }
        else {
            DOMNode* child = parseNodes();
            if (child) {
                m_document->appendChild(child);
            }
        }
    }

    return m_document;
}

DOMNode* HTMLParser::parseNodes()
{
    const Token& t = currentToken();
    if (t.type == TokenType::OpenTag) {
        return parseElement();
    }
    else if (t.type == TokenType::SelfClosingTag) {
        DOMNode* node = new DOMNode(t.value);
        advance(); // consume this token
        return node;
    }
    else if (t.type == TokenType::Text) {
        DOMNode* textNode = new DOMNode("#text");
        textNode->setAttribute("value", t.value);
        advance();
        return textNode;
    }
    else if (t.type == TokenType::CloseTag) {
        std::cerr << "Unexpected close tag " << t.value << "\n";
        advance();
        return nullptr;
    }
    else {
        advance();
        return nullptr;
    }
}

DOMNode* HTMLParser::parseElement()
{
    const Token& t = currentToken();
    std::string tagName = t.value; // e.g. 'div', 'span'
    DOMNode* element = new DOMNode(tagName);

    for (auto &attr : t.attributes) {
        element->setAttribute(attr.first, attr.second);
    }

    advance(); // consume the OpenTag token

    while (!isAtEnd()) {
        const Token& nextT = currentToken();
        if (nextT.type == TokenType::CloseTag && nextT.value == tagName) {
            advance(); // consume the close tag
            break;
        }
        else if (nextT.type == TokenType::CloseTag) {
            std::cerr << "Mismatched close tag for " << tagName << ", got: " << nextT.value << "\n";
            advance();
        }
        else {
            // parse child node
            DOMNode* child = parseNodes();
            if (child) {
                element->appendChild(child);
            }
        }
    }

    return element;
}

bool HTMLParser::isAtEnd() const
{
    return m_index >= m_tokens.size();
}

const Token& HTMLParser::currentToken() const
{
    if (isAtEnd()) {
        static Token eofToken { TokenType::EndOfFile, "" };
        return eofToken;
    }
    return m_tokens[m_index];
}

void HTMLParser::advance()
{
    if (!isAtEnd()) {
        m_index++;
    }
}
