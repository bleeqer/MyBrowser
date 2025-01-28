#include "CommentParser.hpp"

CommentParser::CommentParser(ParserContext& ctx)
    : m_ctx(ctx)
{
}

Token CommentParser::parseComment()
{
    // consume "<!--" => 4 chars
    for (int i = 0; i < 4; i++) {
        m_ctx.get();
    }

    size_t startPos = m_ctx.getPos();
    // read until we see "-->" or end
    while (!m_ctx.isAtEnd()) {
        if (m_ctx.startsWith("-->")) {
            break;
        }
        m_ctx.get();
    }
    size_t endPos = m_ctx.getPos();
    std::string commentText = m_ctx.getHtml().substr(startPos, endPos - startPos);

    // consume '-->' if present
    if (!m_ctx.isAtEnd()) {
        for (int i = 0; i < 3; i++) {
            m_ctx.get();
        }
    }

    Token token;
    token.type = TokenType::Comment;
    token.value = commentText;
    return token;
}
