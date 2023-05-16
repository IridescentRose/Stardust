#include "Tokenizer.hpp"
#include <sstream>
#include <vector>
#include <map>

Token Token::EndOfFileToken = { TokenType::Eof, "", 0, 0 };

Tokenizer::Tokenizer(std::istream& inStream) : in(inStream), currentToken(0) { }

std::map<std::string, TokenType> tokenMap = {
        {"import", TokenType::Import},
        {"var", TokenType::Var},
        {"const", TokenType::Const},
        {"struct", TokenType::Struct},
        {"enum", TokenType::Enum},
        {"while", TokenType::While},
        {"match", TokenType::Match},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"return", TokenType::Return},
        {"pub", TokenType::Pub},
        {"fn", TokenType::Fn},
        {"u8", TokenType::U8},
        {"u16", TokenType::U16},
        {"u32", TokenType::U32},
        {"u64", TokenType::U64},
        {"i8", TokenType::I8},
        {"i16", TokenType::I16},
        {"i32", TokenType::I32},
        {"i64", TokenType::I64},
        {"f32", TokenType::F32},
        {"f64", TokenType::F64},
        {"bool", TokenType::Bool},
        {"true", TokenType::True},
        {"false", TokenType::False},
};

auto Tokenizer::differentiate(const std::string& in) -> TokenType {
    if(tokenMap.find(in) != tokenMap.end())
        return tokenMap[in];
    else
        return TokenType::Identifier;
}

auto Tokenizer::tokenize_line(std::string input, int lineNumber) -> void {
    auto len = input.length();

    std::string current_token;

    auto addRemainingToken = [&](uint32_t i) {
        if (!current_token.empty()) {
            auto t = differentiate(current_token);
            if(t == TokenType::Identifier && current_token[0] >= '0' && current_token[0] <= '9') {
                t = TokenType::Numeric;
            }

            tokens.push_back({t, current_token, static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
            current_token.clear();
        }
    };

    for(auto i = 0; i < len; i++) {
        auto c = input[i];

        switch (c) {

            case ':': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Colon, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '=': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Equal, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '<': {
                addRemainingToken(i);
                tokens.push_back({TokenType::LessThan, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '>': {
                addRemainingToken(i);
                tokens.push_back({TokenType::GreaterThan, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '|': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Or, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '&': {
                addRemainingToken(i);
                tokens.push_back({TokenType::And, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '!': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Not, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '+': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Plus, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '-': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Minus, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '*': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Multiply, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '/': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Divide, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '%': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Modulo, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '^': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Exponent, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '\"': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Quote, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case ',': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Comma, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '\'': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Apostrophe, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '(': {
                addRemainingToken(i);
                tokens.push_back({TokenType::LeftParen, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case ')': {
                addRemainingToken(i);
                tokens.push_back({TokenType::RightParen, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '{': {
                addRemainingToken(i);
                tokens.push_back({TokenType::LeftBrace, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '}': {
                addRemainingToken(i);
                tokens.push_back({TokenType::RightBrace, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case '[': {
                addRemainingToken(i);
                tokens.push_back({TokenType::LeftBracket, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case ']': {
                addRemainingToken(i);
                tokens.push_back({TokenType::RightBracket, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case ';': {
                addRemainingToken(i);
                tokens.push_back({TokenType::Semicolon, std::string(1, c), static_cast<uint32_t>(lineNumber), static_cast<uint32_t>(i)});
                break;
            }

            case ' ':
            case '\t':
            case '\n':
            case '\r':
            case '\f':
            case '\v': {
                addRemainingToken(i);
                break;
            }

            default: {
                current_token += c;
                break;
            }
        }
    }
    addRemainingToken(len);
}

auto Tokenizer::tokenize_all() -> void {
    std::string input;

    int currLine = 1;
    while(std::getline(in, input)) {
        tokenize_line(input, currLine++);
    }
}

auto Tokenizer::next() -> Token {
    if(tokens.size() > currentToken) {
        return tokens[currentToken];
    }

    return Token::EndOfFileToken;
}

auto Tokenizer::skip() -> void {
    currentToken++;
}

auto Tokenizer::get_tokens() -> const std::vector<Token>& {
    return tokens;
}