#include "../dom/DOMDocument.hpp"
//#include "LayoutTree.hpp"

class LayoutEngine
{
public:
    LayoutEngine();

    ~LayoutEngine();

    // Build/update a layout tree from a DOM
    void layout(DOMDocument &doc);

    //    LayoutTree& getLayoutTree();

private:
    //    LayoutTree m_layoutTree;
};
