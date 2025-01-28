#pragma once

#include "ParserContext.hpp"
#include "Token.hpp"

class TextParser {
public:
    TextParser(ParserContext& ctx);

    Token parseText();

private:
    ParserContext& m_ctx;
};
