#include "HTMLLexer.hpp"
#include "CommentParser.hpp"
#include "DoctypeParser.hpp"
#include "TagParser.hpp"
#include "TextParser.hpp"

HTMLLexer::HTMLLexer(ParserContext &ctx) : m_ctx(ctx) {
}

std::vector<Token> HTMLLexer::tokenize()
{
    std::vector<Token> tokens;

    while (!m_ctx.isAtEnd()) {
        if (m_ctx.peek() == '<') {
            if (m_ctx.startsWith("<!--")) {
                CommentParser cp(m_ctx);
                tokens.push_back(cp.parseComment());
            }
            else if (m_ctx.startsWith("<!DOCTYPE")) {
                DoctypeParser dp(m_ctx);
                tokens.push_back(dp.parseDoctype());
            }
            else {
                TagParser tp(m_ctx);
                tokens.push_back(tp.parseTag());
            }
        } else {
            TextParser tp(m_ctx);
            tokens.push_back(tp.parseText());
        }
    }

    Token eofToken;
    eofToken.type = TokenType::EndOfFile;
    tokens.push_back(eofToken);

    return tokens;
}