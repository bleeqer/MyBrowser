#include "Page.hpp"
#include <iostream>
// #include "../parser/HTMLParser.h"

Page::Page()
{
    std::cout << "Creating Page" << std::endl;
}

Page::~Page()
{
    std::cout << "Destroying Page" << std::endl;
}

void Page::loadHTML(HTMLParser &parser, const std::string &html)
{
    DOMDocument doc = parser.parse(html);

    m_document = doc;

    std::cout << "Page loaded HTML of length: " << html.size() << "\n";
}

void Page::update()
{
}


DOMDocument &Page::getDocument()
{
    return m_document;
}
