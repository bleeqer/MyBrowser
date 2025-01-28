#pragma once

#include "ParserContext.hpp"
#include "Token.hpp"

class CommentParser {
public:
    CommentParser(ParserContext& ctx);

    Token parseComment();

private:
    ParserContext& m_ctx;
};
