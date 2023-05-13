#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

enum class TokenType {
    Keyword,
    Identifier,
    Literal,
    Operator,
    Punctuation,
    Unknown
};


auto TokenTypeToString(const TokenType& t) -> std::string {
    std::string res;

    switch(t) {
        case TokenType::Literal: {
            res = "Literal";
            break;
        }
        case TokenType::Keyword: {
            res = "Keyword";
            break;
        }
        case TokenType::Identifier: {
            res = "Identifier";
            break;
        }
        case TokenType::Operator: {
            res = "Operator";
            break;
        }
        case TokenType::Punctuation: {
            res = "Punctuation";
            break;
        }

        default: {
            res = "unknown";
            break;
        }
    }

    return res;
}

struct Token {
    TokenType type = TokenType::Unknown;
    std::string value;
    uint32_t lineNumber = -1;

    [[nodiscard]] auto toString() const -> std::string {
        return R"({"tokenType": ")" + TokenTypeToString(type) + R"(", "value": ")" + value + R"(", "line": )" + std::to_string(lineNumber) + "}";
    }
};

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::stringstream ss(code);
    std::string line;

    int lineCounter = 1;
    while(std::getline(ss, line)) {
        std::stringstream lineStream(line);
        std::string token;

        while(lineStream >> token) {
            Token t;
            t.value = token;

            if(token == "if" || token == "int") {
                t.type = TokenType::Keyword;
            } else if(token == "+" || token == "-" || token == "*" || token == "/" || token == "=" || token == "==" || token == "<" || token == ">" || token == ">=" || token == "<=") {
                t.type = TokenType::Operator;
            } else if(token == ";" || token == "{" || token == "}") {
                t.type = TokenType::Punctuation;
            } else if(token[0] >= '0' && token[0] <= '9') {
                t.type = TokenType::Literal;
            } else {
                t.type = TokenType::Identifier;
            }

            t.lineNumber = lineCounter;
            tokens.push_back(t);
        }

        lineCounter++;
    }

    return tokens;
}

auto jsonify(std::ostream& ostream, const std::vector<Token>& tokens) -> void {
    ostream << "{\n\t\"tokens\": [" << std::endl;

    auto len = tokens.size();
    for(auto i = 0; i < len; i++) {
        auto token = tokens[i];
        ostream << "\t\t" << token.toString();

        if(i != len - 1) {
            ostream << ",";
        }

        ostream << std::endl;
    }
    ostream << "\t]\n}" << std::endl;
}

auto main() -> int {
    std::string testCode = "int x = 5; if (x > 0) { x = x -1; }";
    std::vector<Token> tokens = tokenize(testCode);

    std::ofstream myFile("test.json");

    jsonify(myFile, tokens);

    return 0;
}