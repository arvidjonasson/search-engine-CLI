//
// Created by Arvid Jonasson on 2023-05-19.
//

#ifndef SEARCH_ENGINE_SEARCHENGINE_H
#define SEARCH_ENGINE_SEARCHENGINE_H
#include "Tokenizer.h"
#include "TFIDFComputer.h"
#include "DirectoryManager.h"

class SearchEngine {
public:
    SearchEngine() : directory_manager(verbose), tfidf_computer(verbose) {}

    SearchEngine &addDirectory(const std::filesystem::path &directory_path);

    SearchEngine &compute(const std::wstring &query);

    std::vector<TFIDFComputer::ResultType> search(const std::wstring &query);

    std::vector<TFIDFComputer::ResultType> getResults();

    SearchEngine &clear() noexcept;

    bool verbose = false;
private:
    DirectoryManager directory_manager;
    TFIDFComputer tfidf_computer;
};

#endif //SEARCH_ENGINE_SEARCHENGINE_H
