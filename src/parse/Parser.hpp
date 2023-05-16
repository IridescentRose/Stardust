#pragma once
#include "../Tokenizer.hpp"

class Statement {
public:
    virtual ~Statement() {}
    virtual void execute() = 0;
private:
};

enum class VariableType {
    Auto,
    U8, U16, U32, U64,
    I8, I16, I32, I64,
    F32, F64,
    Bool,
    UserCreated
};

struct TypeInfo {
    VariableType type = VariableType::Auto;
    size_t arraySize = 0;
};

struct Expression : public Statement {
    static auto parse(Tokenizer& tokenizer) -> Expression*;
    inline virtual void execute() {}
    std::vector<Token> tokens;
};

struct StatementSequence : public Statement {
    static auto parse(Tokenizer& tokenizer) -> std::unique_ptr<StatementSequence>;
    inline virtual void execute() {}
    std::vector<std::unique_ptr<Statement>> statements;
};

struct FunctionDeclaration : public Statement {
    static auto parse(Tokenizer& tokenizer, bool pub = false) -> std::unique_ptr<Statement>;

    bool pub = false;
    TypeInfo typeInfo;
    std::string name;
    //std::vector<Arguments> arguments;
    std::unique_ptr<StatementSequence> statementSequence;
    inline virtual void execute() {}
};

//<variable_declaration> ::= <constness> <identifier> ":" <type>
// | <constness> <identifier> [ ":" <type> ] "=" <expression>
struct VariableDeclaration : public Statement {
    static auto parse(Tokenizer& tokenizer, bool pub = false) -> std::unique_ptr<Statement>;

    bool pub = false;
    bool constant = false;
    TypeInfo typeInfo;
    std::string name;
    Expression* expression = nullptr;
    inline virtual void execute() {}
};

struct Program {
    inline auto execute() const -> void {
        for(const auto& statement : statements) {
            statement->execute();
        }
    }

    inline auto addStatement(std::unique_ptr<Statement> statement) -> void {
        statements.push_back(std::move(statement));
    }

    std::vector<std::unique_ptr<Statement>> statements;
};

struct Parser {
    static auto parse(Tokenizer& tokenizer) -> std::unique_ptr<Program>;
};