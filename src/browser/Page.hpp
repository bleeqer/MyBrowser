#pragma once
#include <string>
#include "../dom/DOMDocument.hpp"

class Page {
public:
    Page();
    ~Page();

    void loadHTML(const std::string& html);

    void update();

    DOMDocument& getDocument();

private:
    DOMDocument m_dom;
};
