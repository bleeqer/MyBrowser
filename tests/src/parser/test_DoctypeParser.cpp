#include <gtest/gtest.h>
#include "parser/DoctypeParser.hpp"
#include "parser/ParserContext.hpp"
#include "parser/Token.hpp"

TEST(DoctypeParserTest, BasicDoctype) {
  std::string html = "<!DOCTYPE html>";

  ParserContext ctx(html, 0);

  DoctypeParser parser(ctx);

  Token t = parser.parseDoctype();

  EXPECT_EQ(t.type, TokenType::Doctype);
  EXPECT_EQ(t.value, "html") << "Should contain the substring after <!DOCTYPE";
  EXPECT_TRUE(ctx.isAtEnd()) << "Should have consumed the entire string";
}