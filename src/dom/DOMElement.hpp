#pragma once

#include <string>
#include <map>
#include <vector>

class DOMDocument;

class DOMElement {
public:
    explicit DOMElement(const std::string& tagName);

    DOMDocument* getDocument() const;

    const std::string& getTagName() const;

    void setAttribute(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;

    void appendChild(DOMElement* child);
    const std::vector<DOMElement*>& getChildren() const;

private:
    DOMDocument* m_document;
    std::string m_tagName;
    std::map<std::string, std::string> m_attributes;
    std::vector<DOMElement*> m_children;
};
