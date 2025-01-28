#include "DOMNode.hpp"

DOMNode::DOMNode(const std::string& name)
    : m_name(name)
{
}

DOMNode::~DOMNode()
{

    for (DOMNode* child : m_children) {
        delete child;
    }
}

void DOMNode::appendChild(DOMNode* child)
{
    m_children.push_back(child);
}

void DOMNode::setAttribute(const std::string& key, const std::string& value)
{
    m_attributes[key] = value;
}

const std::string& DOMNode::getName() const
{
    return m_name;
}

bool DOMNode::hasAttribute(const std::string& key) const
{
    return (m_attributes.find(key) != m_attributes.end());
}

std::string DOMNode::getAttribute(const std::string& key) const
{
    auto it = m_attributes.find(key);
    if (it != m_attributes.end()) {
        return it->second;
    }
    return "";
}

std::vector<DOMNode*>& DOMNode::getChildren()
{
    return m_children;
}

const std::vector<DOMNode*>& DOMNode::getChildren() const
{
    return m_children;
}

const std::map<std::string, std::string>& DOMNode::getAttributes() const
{
    return m_attributes;
}
