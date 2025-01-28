#pragma once

#include "ParserContext.hpp"
#include "Token.hpp"
#include "VoidElements.hpp"
#include "Attribute.hpp"
#include <vector>
#include <string>

/**
 * @brief Parses an HTML tag: <tag>, </tag>, or <tag .../>
 */
class TagParser {
public:
    TagParser(ParserContext& ctx);

    Token parseTag();

private:
    enum class TagParseState {
        TagNameState,
        BeforeAttrState,
        AttrNameState,
        AfterAttrNameState,
        BeforeAttrValueState,
        AttrValueState,
        Done
    };

    // Subroutines for each state
    void parseTagName(std::string& tagName, TagParseState& state);
    void parseBeforeAttrState(TagParseState& state);
    void parseAttrName(Attribute& attr, TagParseState& state);
    void parseAfterAttrName(Attribute& attr, TagParseState& state);
    void parseBeforeAttrValue(Attribute& attr, TagParseState& state);
    void parseAttrValue(Attribute& attr, TagParseState& state);

    void skipWhitespace();

    ParserContext& m_ctx;

    // Some local states
    bool m_inQuotedValue = false;
    char m_quoteChar     = '\0';
    bool m_sawSlash      = false; // detect '/>' => selfClosing
};
