//
// Created by Arvid Jonasson on 2023-05-19.
//

#include "Tokenizer.h"
#include <fstream>
#include <iostream>

struct Token {
    std::wstring token;
    bool is_number{};
};

static inline void addToken(Token &currentToken, std::vector<std::wstring> &tokens) {
    if (!currentToken.token.empty()) {
        tokens.push_back(currentToken.token);
        currentToken.token.clear();
    }
}

static inline void parse(wchar_t c, Token &currentToken, std::vector<std::wstring> &tokens) {
    if (iswalpha(c)) {                       // if alpha
        if (currentToken.is_number) {               // if previous token was a number
            addToken(currentToken, tokens);   // add it to the list
            currentToken.is_number = false;         // and reset the flag
        }
        currentToken.token.push_back(std::towlower(c));
    } else if (iswdigit(c)) {                // elif digit
        if (!currentToken.is_number) {              // if previous token was not a number
            addToken(currentToken, tokens);   // add it to the list
            currentToken.is_number = true;          // and set the flag
        }
        currentToken.token.push_back(c);
    } else if(c != L'-' && c != L'\'') {         // elif not a hyphen or apostrophe
        addToken(currentToken, tokens);       // add the token to the list
        //hyphen and apostrophe are ignored
    }
}

std::vector<std::wstring> Tokenizer::tokenize(const std::wstring &input, const bool &verbose) {
    std::vector<std::wstring> tokens;
    Token currentToken;

    if(verbose) {
        std::wcout << L"Tokenizing " << std::quoted(input) << std::endl;
    }

    for (wchar_t c : input) {
        parse(c, currentToken, tokens);
    }
    addToken(currentToken, tokens);

    if(verbose) {
        std::wcout << L"Tokens: [";
        for(size_t idx = 0, size = tokens.size(); idx < size; ++idx) {
            if(idx) std::wcout << L", ";
            std::wcout << tokens[idx];
        }
        std::wcout << L"]" << std::endl;
    }

    return tokens;
}

std::vector<std::wstring> Tokenizer::tokenize(const std::filesystem::path &input, const bool &verbose) {
    if (!exists(input))
        throw std::runtime_error("File does not exist: " + input.string());

    if (!is_regular_file(input))
        throw std::runtime_error("Not a file: " + input.string());

    std::wifstream file(input);
    if (file.fail())
        throw std::runtime_error("Could not open file: " + input.string());

    if(verbose) {
        std::wcout << L"Tokenizing " << std::quoted(input.wstring()) << std::endl;
    }

    std::vector<std::wstring> tokens;
    Token currentToken;

    wchar_t c;
    while (file.get(c)) {
        parse(c, currentToken, tokens);
    }
    addToken(currentToken, tokens);

    if(verbose) {
        std::wcout << L"Total tokens: " << tokens.size() << L'.' << std::endl;
    }

    file.close();
    return tokens;
}


