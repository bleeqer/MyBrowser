#include "ParserContext.hpp"

ParserContext::ParserContext(const std::string& html, size_t startPos)
    : m_html(html)
    , m_pos(startPos)
{
}

const std::string& ParserContext::getHtml() const
{
    return m_html;
}

size_t ParserContext::getPos() const
{
    return m_pos;
}

void ParserContext::setPos(size_t p)
{
    m_pos = p;
}

char ParserContext::peek() const
{
    if (m_pos >= m_html.size()) return '\0';
    return m_html[m_pos];
}

char ParserContext::get()
{
    if (m_pos >= m_html.size()) return '\0';
    return m_html[m_pos++];
}

bool ParserContext::isAtEnd() const
{
    return (m_pos >= m_html.size());
}

bool ParserContext::startsWith(const std::string& str) const
{
    if (m_pos + str.size() > m_html.size()) {
        return false;
    }
    return (m_html.compare(m_pos, str.size(), str) == 0);
}
