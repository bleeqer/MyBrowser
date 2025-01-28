#include "TextParser.hpp"

TextParser::TextParser(ParserContext& ctx)
    : m_ctx(ctx)
{
}

Token TextParser::parseText()
{
    // read until '<' or end
    size_t startPos = m_ctx.getPos();
    while (!m_ctx.isAtEnd() && m_ctx.peek() != '<') {
        m_ctx.get();
    }
    size_t endPos = m_ctx.getPos();

    // substring from startPos..endPos
    std::string text = m_ctx.getHtml().substr(startPos, endPos - startPos);

    Token token;
    token.type = TokenType::Text;
    token.value = text;
    return token;
}
