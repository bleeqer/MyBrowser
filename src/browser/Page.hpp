#pragma once
#include <string>
#include "../dom/DOMDocument.hpp"
#include "../parser/HTMLParser.hpp"

class Page
{
public:
    Page();

    ~Page();

    void loadHTML(HTMLParser &parser, const std::string &html);

    void update();

    DOMDocument &getDocument();

private:
    DOMDocument m_document;
};
