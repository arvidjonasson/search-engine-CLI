//
// Created by Arvid Jonasson on 2023-05-19.
//

#include "SearchEngine.h"

SearchEngine &SearchEngine::addDirectory(const std::filesystem::path &directory_path) {
    directory_manager.addDirectory(directory_path);
    return *this;
}

SearchEngine &SearchEngine::compute(const std::wstring &query) {
    auto new_files = directory_manager.getFiles();
    if(!new_files.empty()) {
        tfidf_computer.addDocuments(new_files);
    }
    auto tokens = Tokenizer::tokenize(query, verbose);
    tfidf_computer.compute(tokens);
    return *this;
}

std::vector<TFIDFComputer::ResultType> SearchEngine::search(const std::wstring &query) {
    compute(query);
    return tfidf_computer.getResults();
}

std::vector<TFIDFComputer::ResultType> SearchEngine::getResults() {
    return tfidf_computer.getResults();
}

SearchEngine &SearchEngine::clear() noexcept {
    directory_manager.clear();
    tfidf_computer.clear();
    return *this;
}
