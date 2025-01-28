#pragma once

#include <string>
#include <map>
#include <vector>

class DOMNode {
public:
    explicit DOMNode(const std::string& name);

    ~DOMNode();

    void appendChild(DOMNode* child);

    void setAttribute(const std::string& key, const std::string& value);

    const std::string& getName() const;

    bool hasAttribute(const std::string& key) const;

    std::string getAttribute(const std::string& key) const;

    std::vector<DOMNode*>& getChildren();

    const std::vector<DOMNode*>& getChildren() const;

    const std::map<std::string, std::string>& getAttributes() const;

private:
    std::string m_name;
    std::map<std::string, std::string> m_attributes;
    std::vector<DOMNode*> m_children;
};
