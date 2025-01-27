#include "DOMElement.hpp"
#include <algorithm>

DOMElement::DOMElement(const std::string& tagName)
    : m_tagName(tagName)
{
}

const std::string& DOMElement::getTagName() const {
    return m_tagName;
}

void DOMElement::setAttribute(const std::string& name, const std::string& value) {
    m_attributes[name] = value;
}

std::string DOMElement::getAttribute(const std::string& name) const {
    auto it = m_attributes.find(name);
    if (it != m_attributes.end()) {
        return it->second;
    }
    return "";
}

void DOMElement::appendChild(DOMElement* child) {
    m_children.push_back(child);
}

const std::vector<DOMElement*>& DOMElement::getChildren() const {
    return m_children;
}
