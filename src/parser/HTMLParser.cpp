#include "HTMLParser.hpp"
#include <stdexcept>
#include <iostream>

DOMDocument HTMLParser::parse(const std::string &html)
{
    // 1) Lex
    HTMLLexer lexer(html);
    m_tokens = lexer.tokenize();
    m_index = 0;

    // 2) Build a DOMDocument
    DOMDocument doc;
    // Possibly create a root <html> if you want a known root

    // 3) Recursively parse elements/text into doc's root
    // For a simple approach, parse everything as children of a single root
    DOMElement *root = doc.createElement("html");
    doc.setRootElement(root);

    parseNodes(root);

    return doc;
}

void HTMLParser::parseNodes(DOMElement *parent)
{
    while (currentToken().type != TokenType::EndOfFile)
    {
        TokenType t = currentToken().type;

        if (t == TokenType::OpenTag)
        {
            parseElement(parent);
        }
        else if (t == TokenType::SelfClosingTag)
        {
        }
        else if (t == TokenType::CloseTag)
        {
            // warning
            std::cerr << "Unexpected close tag: " << currentToken().value << "\n";
            std::cerr << "Auto-inserting open tag\n";
            // TODO:: auto-insert open tag if missing
            return;
        }
        else if (t == TokenType::Text)
        {
        }
        else
        {
            advance();
        }
    }
}

void HTMLParser::parseElement(DOMElement *parent)
{
    // Expect an OpenTag token
    Token openTag = currentToken();
    std::string tagName = openTag.value;
    advance(); // consume the open tag

    // Create the element
    DOMElement *element = parent->getDocument()->createElement(tagName);
    parent->appendChild(element);

    // parse attributes if you had them in tokens (omitted in this minimal example)

    // Now parse children until we see a close tag for this element
    parseNodes(element);

    // We expect a close tag with the same name
    if (currentToken().type == TokenType::CloseTag)
    {
        if (currentToken().value != tagName)
        {
            // mismatch? error or ignore
            std::cerr << "Mismatched close tag for " << tagName << "\n";
        }
        advance(); // consume the close
    }
}

void HTMLParser::advance()
{
    if (m_index < m_tokens.size())
    {
        m_index++;
    }
}

Token HTMLParser::currentToken() const
{
    if (m_index >= m_tokens.size())
    {
        return {TokenType::EndOfFile, ""};
    }
    return m_tokens[m_index];
}
