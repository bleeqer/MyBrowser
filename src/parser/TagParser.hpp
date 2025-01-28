#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <string>
#include <iostream>

enum class TokenType {
    OpenTag,
    CloseTag,
    SelfClosingTag,
    Text,
    Comment,
    Doctype,
    EndOfFile,
    Skip
};

struct Token {
    TokenType type;
    std::string value;
    std::vector<std::pair<std::string, std::string>> attributes;
};

struct Attribute {
    std::string name;
    std::string value;
    bool hasValue = false;
};

class TagParser {
public:
    TagParser(const std::string& html, size_t startPos);

    Token parseTag();

private:
    enum class TagParseState {
        TagNameState,
        BeforeAttrState,
        AttrNameState,
        AfterAttrNameState,
            BeforeAttrValueState,
        AttrValueState,
        DoneState
        };

    char peek() const;
    char get();
    bool isAtEnd() const;
    void skipWhitespace();

    void parseTagName(std::string& tagName, TagParseState& state);
    void parseBeforeAttrState(TagParseState& state);
    void parseAttrName(Attribute& attr, TagParseState& state);
    void parseAfterAttrName(Attribute& attr, TagParseState& state);
    void parseBeforeAttrValue(Attribute& attr, TagParseState& state);
    void parseAttrValue(Attribute& attr, TagParseState& state);

    bool   m_inQuotedValue = false;
    char   m_quoteChar     = '\0';
    bool   m_sawSlash      = false;

    const std::string& m_html;
    size_t m_pos;

};


static const std::unordered_set<std::string> s_voidTags = {
    "area","base","br","col","embed","hr","img","input","link","meta","param",
    "source","track","wbr","command","keygen","menuitem","path","circle","line","rect",
    "svg"
};
