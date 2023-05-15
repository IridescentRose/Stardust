#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "Tokenizer.hpp"

auto jsonify(std::ostream& ostream, const std::vector<Token>& tokens) -> void {
    ostream << "{\n\t\"tokens\": [" << std::endl;

    auto len = tokens.size();
    for(auto i = 0; i < len; i++) {
        auto token = tokens[i];
        ostream << "\t\t" << token.to_string();

        if(i != len - 1) {
            ostream << ",";
        }

        ostream << std::endl;
    }
    ostream << "\t]\n}" << std::endl;
}

auto main(int argc, char** argv) -> int {
    if(argc != 2) {
        std::cerr << "Error: Use only one input file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream src_file(argv[1]);

    Tokenizer tokenizer(src_file);
    tokenizer.tokenize_all();

    std::vector<Token> tokens = tokenizer.get_tokens();

    std::ofstream myFile("test.json");
    jsonify(myFile, tokens);

    return 0;
}