#include "HTMLLexer.hpp"
#include <cctype>
#include <stdexcept>

// Constructor
HTMLLexer::HTMLLexer(const std::string& html)
    : m_html(html)
{
}

std::vector<Token> HTMLLexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        char c = peek();

        if (c == '<') {
            // Look ahead to see if it's a comment, doctype, close tag, etc.
            if (startsWith("<!--")) {
                tokens.push_back(parseComment());
            }
            else if (startsWith("<!DOCTYPE")) {
                tokens.push_back(parseDoctype());
            }
            else if (startsWith("</")) {
                // It's a close tag or possibly </...>
                tokens.push_back(parseTag()); // parseTag handles isClosing if we see '/'
            }
            else {
                // Otherwise, assume it's an open tag
                tokens.push_back(parseTag());
            }
        } else {
            // parse text until next '<'
            tokens.push_back(parseText());
        }
    }

    // EndOfFile token at the end
    tokens.push_back({ TokenType::EndOfFile, "" });
    return tokens;
}

//-------------------
// Parse a TEXT token
//-------------------
Token HTMLLexer::parseText()
{
    size_t start = m_pos;
    // Read until we hit '<' or the end
    while (!isAtEnd() && peek() != '<') {
        get();
    }
    // substring from [start, m_pos)
    std::string text = m_html.substr(start, m_pos - start);
    return { TokenType::Text, text };
}

//------------------------
// Parse <tag> or </tag>
//------------------------
Token HTMLLexer::parseTag()
{
    // Consume the '<'
    get();

    bool isClosing = false;
    if (!isAtEnd() && peek() == '/') {
        isClosing = true;
        get(); // consume '/'
    }

    // Read the tag name (letters, digits, underscores, etc.)
    size_t start = m_pos;
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_' || peek() == '-')) {
        get();
    }
    std::string tagName = m_html.substr(start, m_pos - start);

    // (Optional) skip whitespace, parse attributes, check for '/', etc.
    // For brevity, we just skip until '>'
    while (!isAtEnd() && peek() != '>') {
        get();
    }

    // If we're not at the end, consume '>'
    if (!isAtEnd()) {
        get();
    }

    if (isClosing) {
        // e.g. </div>
        return { TokenType::CloseTag, tagName };
    }
    else {
        // e.g. <div>
        // Could also check if it's self-closing (<br />) but skipping that
        return { TokenType::OpenTag, tagName };
    }
}

//--------------------
// Parse <!-- comment -->
//--------------------
Token HTMLLexer::parseComment()
{
    // We already know startsWith("<!--"), so consume those 4 chars
    // actually we consume '<', '!', '-', '-'
    // or just do a loop of get() 4 times
    for (int i = 0; i < 4; i++) {
        get();
    }
    // Now read until we see '-->'
    size_t start = m_pos;
    while (!isAtEnd()) {
        // If we find "-->", break
        if (startsWith("-->")) {
            break;
        }
        get();
    }
    // The comment text is from start to our current position
    std::string commentText = m_html.substr(start, m_pos - start);

    // If not at the end, consume the "-->"
    if (!isAtEnd()) {
        for (int i = 0; i < 3; i++) {
            get(); // consume '-', '-', '>'
        }
    }

    return { TokenType::Comment, commentText };
}

//-------------------------
// Parse <!DOCTYPE ...>
//-------------------------
Token HTMLLexer::parseDoctype()
{
    // We already know startsWith("<!DOCTYPE")
    // Let's consume 9 characters: '<', '!', 'D', 'O', 'C', 'T', 'Y', 'P', 'E'
    for (int i = 0; i < 9; i++) {
        get();
    }
    // Read until '>' or end
    size_t start = m_pos;
    while (!isAtEnd() && peek() != '>') {
        get();
    }
    std::string doctypeText = m_html.substr(start, m_pos - start);

    if (!isAtEnd()) {
        get(); // consume '>'
    }

    // doctypeText might contain ' html' or something like that
    return { TokenType::Doctype, doctypeText };
}

//----------------------------------
// Helpers to peek, get, and check
//----------------------------------
char HTMLLexer::peek() const
{
    if (m_pos >= m_html.size()) return '\0';
    return m_html[m_pos];
}

char HTMLLexer::peekNext() const
{
    if ((m_pos + 1) >= m_html.size()) return '\0';
    return m_html[m_pos + 1];
}

char HTMLLexer::get()
{
    if (m_pos >= m_html.size()) return '\0';
    return m_html[m_pos++];
}

bool HTMLLexer::isAtEnd() const
{
    return m_pos >= m_html.size();
}

bool HTMLLexer::startsWith(const std::string& str) const
{
    // If there's not enough room left for 'str', return false
    if (m_pos + str.size() > m_html.size()) {
        return false;
    }
    // Compare substring
    return (m_html.compare(m_pos, str.size(), str) == 0);
}
