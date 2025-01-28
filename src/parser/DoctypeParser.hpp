#pragma once

#include "Token.hpp"
#include "ParserContext.hpp"

class DoctypeParser {
public:

    DoctypeParser(ParserContext& ctx);

    Token parseDoctype();

private:
    ParserContext& m_ctx;
};