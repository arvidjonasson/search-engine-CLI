//
// Created by Arvid Jonasson on 2023-05-19.
//
// This class is used to tokenize a string or a file into words.
// The tokens are separated by all non-alphanumeric characters except for hyphen and apostrophe.
// Tokens are separated into either words or numbers. A token can not be both.
// The words are converted to lowercase.

#pragma once
#ifndef SEARCH_ENGINE_TOKENIZER_H
#define SEARCH_ENGINE_TOKENIZER_H

#include <vector>
#include <string>
#include <filesystem>

class Tokenizer {
public:
    [[nodiscard]] static std::vector<std::wstring> tokenize(const std::wstring& input, const bool &verbose = false);

    [[nodiscard]] static std::vector<std::wstring> tokenize(const std::filesystem::path& input, const bool &verbose = false);
};

#endif //SEARCH_ENGINE_TOKENIZER_H
