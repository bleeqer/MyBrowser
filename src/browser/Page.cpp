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

void Page::loadHTML(HTMLParser& parser, const std::string& html)
{
    std::cout << "Loading HTML into Page" << std::endl;
}

void Page::update()
{
    std::cout << "Updating Page" << std::endl;
}

DOMDocument& Page::getDocument()
{
    return m_dom;
}
