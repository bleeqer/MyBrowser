#include "HTMLLexer.hpp"
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

// Constructor
HTMLLexer::HTMLLexer(const std::string &html)
    : m_html(html)
{
}

std::vector<Token> HTMLLexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        char c = peek();

        if (c == '<')
        {
            // Look ahead to see if it's a comment, doctype, close tag, etc.
            if (startsWith("<!--"))
            {
                tokens.push_back(parseComment());
            } else if (startsWith("<!DOCTYPE"))
            {
                tokens.push_back(parseDoctype());
            } else
            {
                Token t = parseTag();

                if (t.type != TokenType::Skip)
                {
                    tokens.push_back(t);
                }
            }
        } else
        {
            // parse text until next '<'
            tokens.push_back(parseText());
        }
    }

    // EndOfFile token at the end
    tokens.push_back({TokenType::EndOfFile, ""});
    return tokens;
}

//-------------------
// Parse a TEXT token
//-------------------
Token HTMLLexer::parseText()
{
    size_t start = m_pos;
    // Read until we hit '<' or the end
    while (!isAtEnd() && peek() != '<')
    {
        get();
    }
    // substring from [start, m_pos)
    std::string text = m_html.substr(start, m_pos - start);
    return {TokenType::Text, text};
}

Token HTMLLexer::parseTag()
{
    get();

    bool isClosing = false;
    bool isSelfClosing = false;
    bool slashBeforeClose = false;

    skipWhitespace();

    if (!isAtEnd() && peek() == '/')
    {
        isClosing = true;
        get(); // consume '/'
    }

    static const std::unordered_set<std::string> voidTags = {
        "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param",
        "source", "track", "wbr", "path", "circle", "line", "rect", "svg", "command", "keygen", "menuitem"
    };


    size_t start = m_pos;

    while (!isAtEnd() && std::isalnum(peek()) || peek() == '-' || peek() == '_')
    {
        get();
    }

    std::string tagName = m_html.substr(start, m_pos - start);

    for (auto &c: tagName)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }

    if (tagName.empty() && isClosing)
    {
        std::cerr << "Warning: empty tag name (< />)\n";

        while (!isAtEnd() && peek() != '>')
        {
            get();
        }

        if (!isAtEnd())
        {
            get(); // consume '>'
        }

        return {TokenType::Skip, ""};
    }

    if (voidTags.find(tagName) != voidTags.end())
    {
        isSelfClosing = true;
    }

    while (!isAtEnd() && peek() != '>')
    {
        if (peek() == '/')
        {
            slashBeforeClose = true;
        }
        get();
    }

    if (!isAtEnd())
    {
        get(); // consume '>'
    }

    isSelfClosing = isSelfClosing || slashBeforeClose;

    if (isClosing)
    {
        return {TokenType::CloseTag, tagName};
    } else if (isSelfClosing)
    {
        return {TokenType::SelfClosingTag, tagName};
    } else
    {
        return {TokenType::OpenTag, tagName};
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
    for (int i = 0; i < 4; i++)
    {
        get();
    }
    // Now read until we see '-->'
    size_t start = m_pos;
    while (!isAtEnd())
    {
        // If we find "-->", break
        if (startsWith("-->"))
        {
            break;
        }
        get();
    }
    // The comment text is from start to our current position
    std::string commentText = m_html.substr(start, m_pos - start);

    // If not at the end, consume the "-->"
    if (!isAtEnd())
    {
        for (int i = 0; i < 3; i++)
        {
            get(); // consume '-', '-', '>'
        }
    }

    return {TokenType::Comment, commentText};
}

//-------------------------
// Parse <!DOCTYPE ...>
//-------------------------
Token HTMLLexer::parseDoctype()
{
    // We already know startsWith("<!DOCTYPE")
    // Let's consume 9 characters: '<', '!', 'D', 'O', 'C', 'T', 'Y', 'P', 'E'
    for (int i = 0; i < 9; i++)
    {
        get();
    }
    // Read until '>' or end
    size_t start = m_pos;
    while (!isAtEnd() && peek() != '>')
    {
        get();
    }
    std::string doctypeText = m_html.substr(start, m_pos - start);

    if (!isAtEnd())
    {
        get(); // consume '>'
    }

    // doctypeText might contain ' html' or something like that
    return {TokenType::Doctype, doctypeText};
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

bool HTMLLexer::startsWith(const std::string &str) const
{
    // If there's not enough room left for 'str', return false
    if (m_pos + str.size() > m_html.size())
    {
        return false;
    }
    // Compare substring
    return (m_html.compare(m_pos, str.size(), str) == 0);
}

void HTMLLexer::skipWhitespace()
{
    while (!isAtEnd() && std::isspace(peek()))
    {
        get();
    }
}

bool HTMLLexer::detectClosingTag()
{
    skipWhitespace();
    if (!isAtEnd() && peek() == '/')
    {
        get(); // consume '/'
        return true;
    }

    return false;
}

std::string HTMLLexer::readTagName()
{
    size_t start = m_pos;
    while (!isAtEnd() && std::isalnum(peek()) || peek() == '-' || peek() == '_')
    {
        get();
    }

    std::string tagName = m_html.substr(start, m_pos - start);

    for (auto &c: tagName)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }

    return tagName;
}

void HTMLLexer::skipMalformedTag()
{
    std::cerr << "Warning: skipping malformed tag\n";

    while (!isAtEnd() && peek() != '>')
    {
        get();
    }

    if (!isAtEnd())
    {
        get(); // consume '>'
    }
}

bool HTMLLexer::detectSelfClosing(const std::string &tagName)
{
    static const std::unordered_set<std::string> voidTags = {
        "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param",
        "source", "track", "wbr", "path", "circle", "line", "rect", "svg", "command", "keygen", "menuitem"
    };

    bool isVoid = voidTags.find(tagName) != voidTags.end();
    bool slashFound = m_sawSlashBeforeClose;

    return isVoid || slashFound;
}
