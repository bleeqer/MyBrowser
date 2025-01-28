#include "DoctypeParser.hpp"
#include "util/StringUtil.cpp"

DoctypeParser::DoctypeParser(ParserContext& ctx) : m_ctx(ctx) {
}

Token DoctypeParser::parseDoctype()
{
    for (int i = 0; i < 9; i++) {
        m_ctx.get();
    }

    size_t start = m_ctx.getPos();
    while (!m_ctx.isAtEnd() && m_ctx.peek() != '>') {
        m_ctx.get();
    }
    std::string docTypeData = m_ctx.getHtml().substr(start, m_ctx.getPos() - start);

    if (!m_ctx.isAtEnd()) {
        m_ctx.get();
    }

    Token token;
    token.type = TokenType::Doctype;
    trim(docTypeData);
    token.value = docTypeData;

    return token;
}

