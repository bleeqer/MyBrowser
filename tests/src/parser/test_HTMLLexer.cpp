#include <gtest/gtest.h>
#include "parser/HTMLLexer.hpp"

/**
 * A test fixture for HTMLLexer tests.
 * Holds optional shared data or helper methods.
 */
class HTMLLexerTest : public ::testing::Test {
protected:
    // Helper to tokenize a string and return the result
    std::vector<Token> tokenize(const std::string& html) {
        HTMLLexer lexer(html);
        return lexer.tokenize();
    }

    // Helper to convert TokenType to string (for debug/logging)
    std::string toString(TokenType type) {
        switch (type) {
            case TokenType::Text:            return "Text";
            case TokenType::OpenTag:         return "OpenTag";
            case TokenType::CloseTag:        return "CloseTag";
            case TokenType::SelfClosingTag:  return "SelfClosingTag";
            case TokenType::Comment:         return "Comment";
            case TokenType::Doctype:         return "Doctype";
            case TokenType::EndOfFile:       return "EndOfFile";
        }
        return "Unknown";
    }
};

/**
 * Utility function to check a single token.
 */
static void checkToken(const Token& actual,
                       TokenType expectedType,
                       const std::string& expectedValue)
{
    EXPECT_EQ(actual.type, expectedType)
        << "Expected token type: " << (int)expectedType
        << ", got: " << (int)actual.type;

    EXPECT_EQ(actual.value, expectedValue)
        << "Expected token value: \"" << expectedValue
        << "\", got: \"" << actual.value << "\"";
}

/**
 * Test that plain text is lexed into a single Text token + EndOfFile.
 */
TEST_F(HTMLLexerTest, BasicText) {
    std::string html = "Hello world";

    auto tokens = tokenize(html);
    ASSERT_EQ(tokens.size(), 2u) << "Should have exactly 2 tokens (Text + EOF).";

    checkToken(tokens[0], TokenType::Text, "Hello world");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

/**
 * Test that <div> is recognized as OpenTag("div") + EndOfFile.
 */
TEST_F(HTMLLexerTest, OpenTag) {
    std::string html = "<div>";

    auto tokens = tokenize(html);
    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::OpenTag, "div");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

/**
 * Test that </span> is recognized as CloseTag("span") + EndOfFile.
 */
TEST_F(HTMLLexerTest, CloseTag) {
    std::string html = "</span>";

    auto tokens = tokenize(html);
    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::CloseTag, "span");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

/**
 * Test that a comment is recognized: <!-- A comment --> => Comment(" A comment ").
 */
TEST_F(HTMLLexerTest, Comment) {
    std::string html = "<!-- A comment -->";

    auto tokens = tokenize(html);
    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::Comment, " A comment ");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

/**
 * Test that <!DOCTYPE html> => Doctype(" html") + EOF.
 */
TEST_F(HTMLLexerTest, Doctype) {
    std::string html = "<!DOCTYPE html>";

    auto tokens = tokenize(html);
    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::Doctype, " html");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

/**
 * A more complex test with multiple elements,
 * whitespace, doctype, and a comment mixed in.
 */
TEST_F(HTMLLexerTest, Combined) {
    std::string html = R"(
        <!DOCTYPE html>
        <!-- a comment -->
        <html>
          <body>Hello <b>world</b></body>
        </html>
    )";

    auto tokens = tokenize(html);
    // We won't check every single token line-by-line,
    // but let's confirm key tokens appear.
    ASSERT_FALSE(tokens.empty());

    bool foundDoctype = false;
    bool foundComment = false;
    bool foundHtmlOpen = false;
    bool foundBodyOpen = false;
    bool foundBOpen = false;
    bool foundBClose = false;
    bool foundHtmlClose = false;

    for (auto& t : tokens) {
        if (t.type == TokenType::Doctype && t.value.find("html") != std::string::npos) {
            foundDoctype = true;
        }
        if (t.type == TokenType::Comment && t.value.find("comment") != std::string::npos) {
            foundComment = true;
        }
        if (t.type == TokenType::OpenTag && t.value == "html") {
            foundHtmlOpen = true;
        }
        if (t.type == TokenType::OpenTag && t.value == "body") {
            foundBodyOpen = true;
        }
        if (t.type == TokenType::OpenTag && t.value == "b") {
            foundBOpen = true;
        }
        if (t.type == TokenType::CloseTag && t.value == "b") {
            foundBClose = true;
        }
        if (t.type == TokenType::CloseTag && t.value == "html") {
            foundHtmlClose = true;
        }
    }

    EXPECT_TRUE(foundDoctype)   << "Should find <!DOCTYPE html> token.";
    EXPECT_TRUE(foundComment)   << "Should find a comment token.";
    EXPECT_TRUE(foundHtmlOpen)  << "Should find <html> token.";
    EXPECT_TRUE(foundBodyOpen)  << "Should find <body> token.";
    EXPECT_TRUE(foundBOpen)     << "Should find <b> token.";
    EXPECT_TRUE(foundBClose)    << "Should find </b> token.";
    EXPECT_TRUE(foundHtmlClose) << "Should find </html> token.";
}
