//
// Created by Arvid Jonasson on 2023-05-19.
//

#include "TFIDFComputer.h"

using Token             = TFIDFComputer::Token;
using PathType          = TFIDFComputer::PathType;
using ResultType        = TFIDFComputer::ResultType;
using ScoreType         = TFIDFComputer::ScoreType;
using FrequencyType     = TFIDFComputer::FrequencyType;
using DocumentType      = TFIDFComputer::DocumentType;

static inline void addWord(const std::wstring& word, std::unordered_map<std::wstring, Token> &tokens,
                           const size_t &documentIndex, const size_t &documentCount) {
    auto& token = tokens.try_emplace(word, Token{
            .frequencies    = std::vector<FrequencyType>(documentCount, 0),
            .doc_frequency  = 0,
            .idf_score      = 0.0
    }).first->second;
    // We'll try to create a new token in place, and if it already exists, we'll get a reference to it.
    // This is more efficient than using contains() and then emplacing if it doesn't exist since we only
    // need to hash the word once.

    if (token.frequencies[documentIndex] == 0) {
        ++token.doc_frequency;
    }
    ++token.frequencies[documentIndex];
    // If the frequency of the word in the document is 0, we'll increment the document frequency of the word
    // and then increment the frequency of the word in the document.
}

static inline void emplaceBackDocuments(const PathType &document, const FrequencyType &totalWords,
                                        std::vector<DocumentType> &documents, std::vector<ResultType> &results) {
    documents.emplace_back(DocumentType{
            .path = document,
            .total_words = totalWords
    });
    results.emplace_back(std::cref(documents.back()));
}

void TFIDFComputer::clear() noexcept {
    tokens          .clear();
    results         .clear();
    documents       .clear();
}

std::vector<ResultType> TFIDFComputer::getResults() const {
    return results;
}

void TFIDFComputer::compute(const std::vector<std::wstring> &words) {
    for(auto& document : documents) {
        document.tfidf_score = 0.0;
    }

    for(const auto& word : words) {
        if(tokens.contains(word)) {
            const auto& token = tokens.at(word);
            for(size_t index = 0; index < documents.size(); ++index) {
                auto tf = static_cast<ScoreType>(token.frequencies[index]) / documents[index].total_words;
                documents[index].tfidf_score += tf * token.idf_score;
            }
        }
    }

    std::sort(results.begin(), results.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.get().tfidf_score > rhs.get().tfidf_score;
    });
}

void TFIDFComputer::addDocuments(const std::vector<PathType> &documents) {
    size_t newDocumentCount = this->documents.size() + documents.size();

    for (auto &[word, token]: tokens)
        token.frequencies.resize(newDocumentCount, 0);
    // We'll resize the frequency vector of each token to fit the new document count.

    results         .reserve(newDocumentCount);
    this->documents .reserve(newDocumentCount);
    // We'll reserve enough space for the new documents in the results and documents vectors.
    // This is purely for optimization.

    for(size_t documentIndex = 0; auto &document : documents) {
        auto words = Tokenizer::tokenize(document, verbose);
        emplaceBackDocuments(document, words.size(), this->documents, results);
        // We'll add the document to the documents vector and add a reference to it to the results vector.
        for (auto &word: words)
            addWord(word, tokens, documentIndex, newDocumentCount);
        // We'll increment the count of each word in the document. This will write to the newly created
        // indicies in the frequency vector of each token.
        ++documentIndex;
    }

    for(auto& [word, token] : tokens) {
        token.idf_score = std::log(newDocumentCount / token.doc_frequency);
    }
}

void TFIDFComputer::addDocument(const PathType &document) {
    addDocuments({document});
}

