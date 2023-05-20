#include <iostream>
#include <locale>
#include <iomanip>
#include <string>

#include "SearchEngine.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    std::wcin.imbue(std::locale(""));

    SearchEngine searchEngine;

    std::wstring directory_path;
    std::wcout << L"Enter a directory: ";
    std::getline(std::wcin, directory_path);

    while (!std::filesystem::is_directory(directory_path)) {
        if(!std::filesystem::exists(directory_path)) {
            std::wcout << std::quoted(directory_path) << L" doesn't exist.\n";
        } else {
            std::wcout << std::quoted(directory_path) << L" isn't a directory.\n";
        }
        std::wcout << L"Supply another one: ";
        std::getline(std::wcin, directory_path);
    }
    searchEngine.addDirectory(directory_path);

    std::wstring query;

    std::wcout << L"Supply a search query: ";
    std::getline(std::wcin, query);

    auto results = searchEngine.search(query);

    for(auto &entry : results) {
        auto &unwrapped = entry.get();
        std::wcout << L"File: "         << unwrapped.path.filename().wstring()
                   << ", tf-idf: "      << unwrapped.tfidf_score
                   << ", total words: " << unwrapped.total_words << std::endl;
    }


    results = searchEngine.addDirectory(std::filesystem::path(directory_path).parent_path())
            // Will skip the folder we've already added, thus we don't reach a cycle and files aren't included twice
            .compute(L"Doesn't split double-words or at123 nor includes !non-alnum; chars.")
            // Will be parsed as [doesnt, split, doublewords, or, at, 123, nor, includes, nonalnum, chars]
            .getResults();

    for(auto &entry : results) {
        auto &unwrapped = entry.get();
        std::wcout << L"File: "         << unwrapped.path.filename().wstring()
                   << ", tf-idf: "      << unwrapped.tfidf_score
                   << ", total words: " << unwrapped.total_words << std::endl;
    }

    return 0;

}
