//
// Created by Arvid Jonasson on 2023-05-19.
//
// This class will compute the tf-idf score for a set of documents.
// The tf-idf score is a measure of how important a word is to a document in a collection of documents.
// The tf-idf score is computed by multiplying the term frequency (tf) with the inverse document frequency (idf).
// The term frequency is the number of times a word occurs in a document divided by the total number of words in the document.
// The inverse document frequency is the logarithm of the total number of documents divided by the number of documents containing the word.
// The tf-idf score is incremented by each word in the query, and then the documents are sorted by their score.
//

#ifndef SEARCH_ENGINE_TFIDFCOMPUTER_H
#define SEARCH_ENGINE_TFIDFCOMPUTER_H
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <fstream>
#include <cmath>

#include "Tokenizer.h"

class TFIDFComputer {
public:
    struct DocumentType;

    using FrequencyType = uint_least32_t;
    using ScoreType     = double;
    using PathType      = std::filesystem::path;
    using ResultType    = std::reference_wrapper<const DocumentType>;

    struct Token {
        std::vector<FrequencyType> frequencies;
        FrequencyType doc_frequency{};
        ScoreType idf_score{};
    };

    struct DocumentType {
        std::filesystem::path path;
        ScoreType tfidf_score{};
        FrequencyType total_words{};
    };

    TFIDFComputer(const bool &verbose) : verbose(verbose) {}

    void clear() noexcept;

    void addDocuments(const std::vector<PathType> &documents);

    void addDocument(const PathType &document);

    [[nodiscard]] std::vector<ResultType> getResults() const;

    void compute(const std::vector<std::wstring>& words);

private:
    std::unordered_map<std::wstring, Token> tokens;
    std::vector<DocumentType> documents;
    std::vector<ResultType> results;
    const bool &verbose = false;
};


#endif //SEARCH_ENGINE_TFIDFCOMPUTER_H
