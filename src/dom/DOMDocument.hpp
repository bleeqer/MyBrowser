#pragma once

#include <string>
#include "DOMElement.hpp"

class DOMDocument {
public:
    DOMDocument();
    ~DOMDocument();

    DOMElement* createElement(const std::string& tagName);

    void setRootElement(DOMElement* root);

    DOMElement* getRootElement() const;

private:
    DOMElement* m_root;
};
