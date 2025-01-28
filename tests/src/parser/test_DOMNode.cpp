#include <gtest/gtest.h>
#include "parser/DOMNode.hpp"

class DOMNodeTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(DOMNodeTest, CreateNodeWithName) {

    DOMNode* node = new DOMNode("div");

    EXPECT_EQ(node->getName(), "div");

    EXPECT_TRUE(node->getChildren().empty());

    EXPECT_TRUE(node->getAttributes().empty());

    delete node;
}

TEST_F(DOMNodeTest, AppendChildNodes) {
    DOMNode* root = new DOMNode("body");
    DOMNode* child1 = new DOMNode("div");
    DOMNode* child2 = new DOMNode("#text");

    root->appendChild(child1);
    root->appendChild(child2);

    std::vector<DOMNode*> children = root->getChildren();

    EXPECT_EQ(children.size(), 2u);
    EXPECT_EQ(children[0]->getName(), "div");
    EXPECT_EQ(children[1]->getName(), "#text");

    delete root;
}

TEST_F(DOMNodeTest, SetAndGetAttributes) {
    DOMNode* node = new DOMNode("img");

    node->setAttribute("src", "image.png");
    node->setAttribute("alt",  "An image");
    node->setAttribute("width", "100");

    EXPECT_TRUE(node->hasAttribute("src"));
    EXPECT_TRUE(node->hasAttribute("alt"));
    EXPECT_TRUE(node->hasAttribute("width"));
    EXPECT_FALSE(node->hasAttribute("height"));

    EXPECT_EQ(node->getAttribute("src"), "image.png");
    EXPECT_EQ(node->getAttribute("alt"), "An image");
    EXPECT_EQ(node->getAttribute("width"), "100");
    EXPECT_EQ(node->getAttribute("height"), "");

    node->setAttribute("width","200");
    EXPECT_EQ(node->getAttribute("width"), "200");
}