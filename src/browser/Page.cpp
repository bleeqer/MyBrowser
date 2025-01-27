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

void Page::loadHTML(const std::string& html)
{
    std::cout << "Loading HTML: " << html << std::endl;
    // HTMLParser parser;
    // m_dom = parser.parse(html); 
}

void Page::update()
{
    std::cout << "Updating Page" << std::endl;
}

DOMDocument& Page::getDocument()
{
    return m_dom;
}
