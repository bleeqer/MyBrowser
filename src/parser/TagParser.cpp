#include "TagParser.hpp"

TagParser::TagParser(const std::string& html, size_t startPos) : m_html(html), m_pos(startPos)
{
}

Token TagParser::parseTag()