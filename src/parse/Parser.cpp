#include "Parser.hpp"

auto throwError(const std::string& error, Token token) -> void {
    std::cerr << "Error: " << error << token.line << ":" << token.column << std::endl;
    throw std::runtime_error("Compilation failed!");
}

auto getVarTypeFromToken(Token token) -> VariableType {
    switch(token.type) {
        case TokenType::U8:
            return VariableType::U8;

        case TokenType::U16:
            return VariableType::U16;

        case TokenType::U32:
            return VariableType::U32;

        case TokenType::U64:
            return VariableType::U64;

        case TokenType::I8:
            return VariableType::I8;

        case TokenType::I16:
            return VariableType::I16;

        case TokenType::I32:
            return VariableType::I32;

        case TokenType::I64:
            return VariableType::I64;

        case TokenType::F32:
            return VariableType::F32;

        case TokenType::F64:
            return VariableType::F64;

        case TokenType::Bool:
            return VariableType::Bool;

        case TokenType::Identifier:
            return VariableType::UserCreated;

        default: {
            break;
        }
    }

    throwError("Unknown type at ", token);
    return VariableType::Auto;
}

auto Expression::parse(Tokenizer &tokenizer) -> Expression * {
    auto* expr = new Expression();

    Token tok;
    while((tok = tokenizer.next()).type != TokenType::Semicolon) {
        expr->tokens.push_back(tok);
        tokenizer.skip();
    }

    return expr;
}

auto StatementSequence::parse(Tokenizer &tokenizer) -> std::unique_ptr<StatementSequence> {
    auto* ss = new StatementSequence();

    while(tokenizer.next().type != TokenType::RightBrace) {
        auto s = std::unique_ptr<Statement>(reinterpret_cast<Statement*>(Expression::parse(tokenizer)));
        ss->statements.push_back(std::move(s));

        tokenizer.skip();
    }

    return std::unique_ptr<StatementSequence>(ss);
}

auto FunctionDeclaration::parse(Tokenizer& tokenizer, bool pub) -> std::unique_ptr<Statement> {
    auto* fd = new FunctionDeclaration();
    fd->pub = pub;

    tokenizer.skip();
    Token tok = tokenizer.next();

    if(tok.type != TokenType::Identifier)
        throwError("Unexpected token at ", tok);

    fd->name = tok.value;

    tokenizer.skip();
    tok = tokenizer.next();

    if(tok.type != TokenType::LeftParen)
        throwError("Unexpected token at ", tok);

    tokenizer.skip();
    tok = tokenizer.next();

    if(tok.type != TokenType::RightParen) {
        throwError("ARGS UNIMPLEMENTED ", tok);
    }

    tokenizer.skip();
    tok = tokenizer.next();

    if(tok.type != TokenType::LeftBrace) {
        //Could be a type
        if(tok.type != TokenType::U8 && tok.type != TokenType::I8 &&
                tok.type != TokenType::U16 && tok.type != TokenType::I16 &&
                tok.type != TokenType::U32 && tok.type != TokenType::I32 &&
                tok.type != TokenType::U64 && tok.type != TokenType::I64 &&
                tok.type != TokenType::F32 && tok.type != TokenType::F64 &&
                tok.type != TokenType::Bool) {
            throwError("Unexpected token at ", tok);
        }

        fd->typeInfo.type = getVarTypeFromToken(tok);

        tokenizer.skip();
        tok = tokenizer.next();

        if(tok.type != TokenType::LeftBrace) {
            throwError("Expected '{' at ", tok);
        }
    }

    tokenizer.skip();

    fd->statementSequence = StatementSequence::parse(tokenizer);

    if(tokenizer.next().type != TokenType::RightBrace) {
        throwError("Expected '}' at ", tokenizer.next());
    }
    tokenizer.skip();

    return std::unique_ptr<Statement>(reinterpret_cast<Statement*>(fd));
}


auto VariableDeclaration::parse(Tokenizer& tokenizer, bool pub) -> std::unique_ptr<Statement> {
    auto* vd = new VariableDeclaration();
    vd->pub = pub;

    auto tok = tokenizer.next();
    vd->constant = tok.type == TokenType::Const;

    tokenizer.skip();
    tok = tokenizer.next();

    if(tok.type != TokenType::Identifier) {
        throwError("Couldn't find identifier at ", tok);
    }

    vd->name = tok.value;

    tokenizer.skip();
    tok = tokenizer.next();

    if(tok.type != TokenType::Colon && tok.type != TokenType::Equal) {
        throwError("Unexpected token at ", tok);
    }

    if(tok.type == TokenType::Colon) {
        tokenizer.skip();
        tok = tokenizer.next();

        vd->typeInfo.type = getVarTypeFromToken(tok);

        tokenizer.skip();
        tok = tokenizer.next();
    }

    if(tok.type == TokenType::Equal) {
        tokenizer.skip();

        vd->expression = Expression::parse(tokenizer);
    }

    if(tokenizer.next().type == TokenType::Semicolon) {
        tokenizer.skip();
    } else {
        throwError("Expected ';' at ", tokenizer.next());
    }

    return std::unique_ptr<Statement>(reinterpret_cast<Statement*>(vd));
}

auto Parser::parse(Tokenizer& tokens) -> std::unique_ptr<Program> {
    auto program = new Program();

    //<program> ::= [<import_statements>] [<global_variable_declarations>]
    // [<global_structure_declarations>] [<global_enum_declarations>]
    // [<function_declarations>]

    Token token;
    while((token = tokens.next()).type != TokenType::Eof) {

        if(token.type == TokenType::Import) {
            // Import statement
        } else if (token.type == TokenType::Pub) {
            // Public:
            // Variable, Struct, Enum, Function

            tokens.skip();
            token = tokens.next();

            if (token.type == TokenType::Var || token.type == TokenType::Const) {
                // Variable Declaration
                program->addStatement(VariableDeclaration::parse(tokens, true));
            } else if (token.type == TokenType::Struct) {
                // Struct Declaration
                throwError("UNIMPLEMENTED ", token);
            } else if (token.type == TokenType::Enum) {
                // Enum Declaration
                throwError("UNIMPLEMENTED ", token);
            } else if (token.type == TokenType::Fn) {
                // Function Declaration
                program->addStatement(FunctionDeclaration::parse(tokens, true));
            }
        } else if (token.type == TokenType::Var || token.type == TokenType::Const) {
            // Variable Declaration
            program->addStatement(VariableDeclaration::parse(tokens));
        } else if (token.type == TokenType::Struct) {
            // Struct Declaration
            throwError("UNIMPLEMENTED ", token);
        } else if (token.type == TokenType::Enum) {
            // Enum Declaration
            throwError("UNIMPLEMENTED ", token);
        } else if (token.type == TokenType::Fn) {
            // Function Declaration
            program->addStatement(FunctionDeclaration::parse(tokens));
        } else {
            throwError("Unexpected token at ", token);
        }
    }

    return std::move(std::unique_ptr<Program>(program));
}