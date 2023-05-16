#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

enum class TokenType {
    Unknown = -1,
    Eof,
    Import,
    Var,
    Const,
    Struct,
    Enum,
    While,
    Match,
    If,
    Else,
    Return,
    Pub,
    Fn,
    U8, U16, U32, U64,
    I8, I16, I32, I64,
    F32, F64,
    Bool,
    True,
    False,
    Numeric,
    Semicolon,
    LeftParen,
    LeftBracket,
    LeftBrace,
    RightParen,
    RightBracket,
    RightBrace,
    Colon,
    Equal,
    LessThan,
    GreaterThan,
    Or,
    And,
    Not,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Exponent,
    Quote,
    Apostrophe,
    Comma,
    Identifier
};

inline auto token_to_string(TokenType type) -> std::string {
    switch(type) {
        case TokenType::Unknown:
            return "unknown";
            
        case TokenType::Eof:
            return "EOF";
            
        case TokenType::Import:
            return "Import";
            
        case TokenType::Var:
            return "Var";
            
        case TokenType::Const:
            return "Const";

        case TokenType::Bool:
            return "Bool";

        case TokenType::Semicolon:
            return "Semicolon";

        case TokenType::Struct:
            return "Struct";
            
        case TokenType::Enum:
            return "Enum";
            
        case TokenType::While:
            return "While";
            
        case TokenType::Match:
            return "Match";
            
        case TokenType::If:
            return "If";
            
        case TokenType::Else:
            return "Else";
            
        case TokenType::Return:
            return "Return";

        case TokenType::Numeric:
            return "Numeric";

        case TokenType::Pub:
            return "Pub";
            
        case TokenType::Fn:
            return "Fn";
            
        case TokenType::U8:
            return "U8";
            
        case TokenType::U16:
            return "U16";
            
        case TokenType::U32:
            return "U32";
            
        case TokenType::U64:
            return "U64";
            
        case TokenType::I8:
            return "I8";
            
        case TokenType::I16:
            return "I16";
            
        case TokenType::I32:
            return "I32";
            
        case TokenType::I64:
            return "I64";
            
        case TokenType::F32:
            return "F32";
            
        case TokenType::F64:
            return "F64";
            
        case TokenType::True:
            return "True";
            
        case TokenType::False:
            return "False";
            
        case TokenType::LeftParen:
            return "LeftParen";
            
        case TokenType::LeftBracket:
            return "LeftBracket";
            
        case TokenType::LeftBrace:
            return "LeftBrace";
            
        case TokenType::RightParen:
            return "RightParen";
            
        case TokenType::RightBracket:
            return "RightBracket";
            
        case TokenType::RightBrace:
            return "RightBrace";
            
        case TokenType::Colon:
            return "Colon";
            
        case TokenType::Equal:
            return "Equal";
            
        case TokenType::LessThan:
            return "LessThan";
            
        case TokenType::GreaterThan:
            return "GreaterThan";
            
        case TokenType::Or:
            return "Or";
            
        case TokenType::And:
            return "And";
            
        case TokenType::Not:
            return "Not";
            
        case TokenType::Plus:
            return "Plus";
            
        case TokenType::Minus:
            return "Minus";
            
        case TokenType::Multiply:
            return "Multiply";
            
        case TokenType::Divide:
            return "Divide";
            
        case TokenType::Modulo:
            return "Modulo";
            
        case TokenType::Exponent:
            return "Exponent";
            
        case TokenType::Quote:
            return "Quote";
            
        case TokenType::Apostrophe:
            return "Apostrophe";
            
        case TokenType::Comma:
            return "Comma";
            
        case TokenType::Identifier:
            return "Identifier";
            
    }

    return "unknown";
}

struct Token {
    Token() = default;

    TokenType type = TokenType::Unknown;
    std::string value;

    // Metadata
    uint32_t line = 0;
    uint32_t column = 0;

    [[nodiscard]] inline auto to_string() const -> std::string {
        return R"({"tokenType": ")" + token_to_string(type) + R"(", "value": ")" + value + R"(", "line": )" + std::to_string(line) + R"(, "column": )" + std::to_string(column) + "}";
    }

    static Token EndOfFileToken;
};

class Tokenizer {
public:
    explicit Tokenizer(std::istream& inStream);

    auto tokenize_all() -> void;
    auto next() -> Token;
    auto skip() -> void;
    auto get_tokens() -> const std::vector<Token>&;
private:
    auto tokenize_line(std::string input, int lineNumber) -> void;
    static auto differentiate(const std::string& in) -> TokenType;

    uint32_t currentToken;
    std::vector<Token> tokens;
    std::istream& in;
};