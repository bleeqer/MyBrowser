#pragma once

enum class TokenType {
    OpenTag,
    CloseTag,
    SelfClosingTag,
    Text,
    Comment,
    Doctype,
    EndOfFile,
    Skip
};
