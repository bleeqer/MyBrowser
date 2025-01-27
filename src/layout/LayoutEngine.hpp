#pragma once
#include "../dom/DOMDocument.h"
#include "LayoutTree.h"  // If you have a LayoutTree class

class LayoutEngine {
public:
    LayoutEngine();
    ~LayoutEngine();

    // Build/update a layout tree from a DOM
    void layout(DOMDocument& doc);

    LayoutTree& getLayoutTree();

private:
    LayoutTree m_layoutTree;
};
