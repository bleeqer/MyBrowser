#include "DOMDocument.hpp"
#include <iostream>

DOMDocument::DOMDocument()
    : m_root(nullptr)
{
}

DOMDocument::~DOMDocument()
{
    delete m_root;
}

DOMElement* DOMDocument::createElement(const std::string& tagName)
{
    DOMElement* elem = new DOMElement(tagName);
    return elem;
}

void DOMDocument::setRootElement(DOMElement* root)
{
    if (m_root) {
        delete m_root;
    }
    m_root = root;
}

DOMElement* DOMDocument::getRootElement() const
{
    return m_root;
}
