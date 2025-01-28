#include "TagParser.hpp"
#include <cctype>
#include <algorithm>

TagParser::TagParser(ParserContext& ctx)
    : m_ctx(ctx)
{
}

Token TagParser::parseTag()
{
    // consume '<'
    m_ctx.get();

    // check if closing: </...
    bool isClosing = false;
    if (!m_ctx.isAtEnd() && m_ctx.peek() == '/') {
        isClosing = true;
        m_ctx.get();
    }

    std::string tagName;
    std::vector<Attribute> attributes;
    TagParseState state = TagParseState::TagNameState;

    while (!m_ctx.isAtEnd() && state != TagParseState::Done) {
        switch (state) {
            case TagParseState::TagNameState:
                parseTagName(tagName, state);
                break;
            case TagParseState::BeforeAttrState:
                parseBeforeAttrState(state);
                break;
            case TagParseState::AttrNameState: {
                attributes.push_back(Attribute{});
                parseAttrName(attributes.back(), state);
                break;
            }
            case TagParseState::AfterAttrNameState:
                parseAfterAttrName(attributes.back(), state);
                break;
            case TagParseState::BeforeAttrValueState:
                parseBeforeAttrValue(attributes.back(), state);
                break;
            case TagParseState::AttrValueState:
                parseAttrValue(attributes.back(), state);
                break;
            case TagParseState::Done:
                break;
        }
    }

    // Possibly consume '>'
    if (!m_ctx.isAtEnd()) {
        if (m_ctx.peek() == '>') {
            m_ctx.get();
        }
    }

    // Lowercase the tagName
    for (auto &ch : tagName) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    // Check if it's a void element
    bool isVoid = (s_voidTags.find(tagName) != s_voidTags.end());
    bool selfClosing = (m_sawSlash || isVoid);

    Token token;
    if (isClosing) {
        token.type = TokenType::CloseTag;
    } else if (selfClosing) {
        token.type = TokenType::SelfClosingTag;
    } else {
        token.type = TokenType::OpenTag;
    }
    token.value = tagName;

    // Move attributes into token
    for (auto &attr : attributes) {
        token.attributes.push_back({attr.name, attr.value});
    }

    return token;
}

//---------------------------------------------
// State subroutines
//---------------------------------------------
void TagParser::parseTagName(std::string& tagName, TagParseState& state)
{
    skipWhitespace();
    while (!m_ctx.isAtEnd()) {
        char c = m_ctx.peek();
        if (std::isalnum(static_cast<unsigned char>(c)) || c=='-' || c=='_') {
            tagName.push_back(c);
            m_ctx.get();
        } else {
            break;
        }
    }
    state = TagParseState::BeforeAttrState;
}

void TagParser::parseBeforeAttrState(TagParseState& state)
{
    skipWhitespace();
    if (m_ctx.isAtEnd()) {
        state = TagParseState::Done;
        return;
    }
    char c = m_ctx.peek();
    if (c == '>') {
        state = TagParseState::Done;
    }
    else if (c == '/') {
        m_ctx.get();
        m_sawSlash = true;
        state = TagParseState::Done;
    } else {
        state = TagParseState::AttrNameState;
    }
}

void TagParser::parseAttrName(Attribute& attr, TagParseState& state)
{
    skipWhitespace();
    while (!m_ctx.isAtEnd()) {
        char c = m_ctx.peek();
        if (std::isalnum(static_cast<unsigned char>(c)) || c=='-' || c=='_') {
            attr.name.push_back(c);
            m_ctx.get();
        } else {
            break;
        }
    }
    state = TagParseState::AfterAttrNameState;
}

void TagParser::parseAfterAttrName(Attribute& attr, TagParseState& state)
{
    skipWhitespace();
    if (m_ctx.isAtEnd()) {
        state = TagParseState::Done;
        return;
    }
    char c = m_ctx.peek();
    if (c == '=') {
        m_ctx.get();
        attr.hasValue = true;
        state = TagParseState::BeforeAttrValueState;
    }
    else if (c == '>') {
        state = TagParseState::Done;
    }
    else if (c == '/') {
        m_ctx.get();
        m_sawSlash = true;
        state = TagParseState::Done;
    }
    else {
        // new attribute or whitespace
        state = TagParseState::AttrNameState;
    }
}

void TagParser::parseBeforeAttrValue(Attribute& attr, TagParseState& state)
{
    skipWhitespace();
    if (m_ctx.isAtEnd()) {
        state = TagParseState::Done;
        return;
    }
    char c = m_ctx.peek();
    if (c == '\"' || c == '\'') {
        m_ctx.get();
        m_inQuotedValue = true;
        m_quoteChar = c;
        state = TagParseState::AttrValueState;
    }
    else if (c == '>' || c == '/') {
        // no value => boolean attribute
        state = TagParseState::BeforeAttrState;
    } else {
        m_inQuotedValue = false;
        m_quoteChar = '\0';
        state = TagParseState::AttrValueState;
    }
}

void TagParser::parseAttrValue(Attribute& attr, TagParseState& state)
{
    if (m_inQuotedValue) {
        while (!m_ctx.isAtEnd()) {
            char c = m_ctx.peek();
            if (c == m_quoteChar) {
                m_ctx.get(); // consume quote
                break;
            }
            attr.value.push_back(c);
            m_ctx.get();
        }
    } else {
        // unquoted => read until whitespace, '>', or '/'
        while (!m_ctx.isAtEnd()) {
            char c = m_ctx.peek();
            if (std::isspace(static_cast<unsigned char>(c)) || c=='>' || c=='/') {
                break;
            }
            attr.value.push_back(c);
            m_ctx.get();
        }
    }
    state = TagParseState::BeforeAttrState;
}

void TagParser::skipWhitespace()
{
    while (!m_ctx.isAtEnd() && std::isspace(static_cast<unsigned char>(m_ctx.peek()))) {
        m_ctx.get();
    }
}
