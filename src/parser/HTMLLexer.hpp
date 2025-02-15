#pragma once
#include "ParserContext.hpp"
#include <vector>
#include "Token.hpp"

class HTMLLexer {
public:
    HTMLLexer(ParserContext& ctx);

    std::vector<Token> tokenize();

private:
    ParserContext& m_ctx;
};