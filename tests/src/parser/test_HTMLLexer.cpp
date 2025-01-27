#include <gtest/gtest.h>
#include "parser/HTMLLexer.hpp"

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

TEST_F(HTMLLexerTest, BasicText) {
    std:: string html = "Hello world";

    auto tokens = tokenize(html);

    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::Text, "Hello world");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}

TEST_F(HTMLLexerTest, OpenTag) {
    std::string html = "<div>";

    auto tokens = tokenize(html);

    ASSERT_EQ(tokens.size(), 2u);

    checkToken(tokens[0], TokenType::OpenTag, "div");
    checkToken(tokens[1], TokenType::EndOfFile, "");
}