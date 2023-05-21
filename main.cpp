#include <iostream>
#include <locale>
#include <iomanip>
#include <string>
#include <algorithm>

#include "SearchEngine.h"

static constexpr size_t MAX_RESULTS = 10;
static constexpr size_t FEW_RESULTS = 5;

static inline auto tolower(const std::wstring &str) {
    std::wstring res;

    for(const auto &c : str) {
        if(std::iswspace(c)) continue;
        res.push_back(towlower(c));
    }
    return res;
}

static inline void addDirectory(SearchEngine &searchEngine) {
    std::wstring directory_path;
    std::wcout << L"Enter a directory: ";
    std::getline(std::wcin, directory_path);

    while (!std::filesystem::is_directory(directory_path)) {
        if(!std::filesystem::exists(directory_path)) {
            std::wcout << std::quoted(directory_path) << L" doesn't exist.\n";
        } else {
            std::wcout << std::quoted(directory_path) << L" isn't a directory.\n";
        }
        std::wcout << L"Type \"cancel\" to return." << std::endl;
        std::wcout << L"Supply another one: ";
        std::getline(std::wcin, directory_path);
        if(tolower(directory_path) == L"cancel") return;
    }
    searchEngine.addDirectory(directory_path);
}

static inline void search(SearchEngine &searchEngine) {
    std::wstring query;

    std::wcout << L"Supply a search query: ";
    std::getline(std::wcin, query);

    auto results = searchEngine.search(query);

    if(results.empty()) {
        std::wcout << L"No files to be searched. Supply a directory with valid files first." << std::endl;
    }

    for(size_t entries = 0; auto &entry : results) {
        if(entries >= MAX_RESULTS) break;

        auto &unwrapped = entry.get();
        if(unwrapped.tfidf_score == 0) {
            if(!entries) {
                std::wcout << L"No matches found. Try another query or set of files." << std::endl;
            } else if(entries < FEW_RESULTS) {
                std::wcout << L"No more results found." << std::endl;
            }
            break;
        }

        std::wcout << L"Score: "      << unwrapped.tfidf_score
                   << L", file: "         << unwrapped.path.filename().wstring()
                   << L", total words: " << unwrapped.total_words << std::endl;

        ++entries;
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    std::wcin.imbue(std::locale(""));

    SearchEngine searchEngine;

    std::wcout << L"Search Engine CLI uses a tf-idf algorithm to serve the most relevant documents\n"
    << L"It also works with non-english alphabets if they're defined in your standard 'locale'.\n"
    << L'\n'
    << L"Here is how to use it:\n"
    << L"    1. Supply the directories you want to search by using the -d flag and pressing enter.\n"
    << L"       The program will recursively search through all subfolders.\n"
    << L"    2. Supply a search query by using the -s flag and pressing enter.\n"
    << L"       The program will compute and serve the paths to the most relevant files.\n"
    << L"    3. Supply more queries or directories as you wish.\n"
    << L"    4. Use the -c flag to clear the current directories.\n"
    << L"Use the -v flag to toggle verbose on/off.\n"
    << std::endl;

    while (true) {
        std::wstring input;
        std::wcout << L'\n'
                   << L"[-d] = supply a directory\n"
                   << L"[-s] = search\n"
                   << L"[-c] = clear directories\n"
                   << L"[-v] = toggle verbose\n"
                   << L"[-q] = quit\n"
                   << L"Supply a command: ";
        std::getline(std::wcin, input);
        input = tolower(input);

        if(input == L"-d") {
            addDirectory(searchEngine);
        } else if(input == L"-s") {
            search(searchEngine);
        } else if(input == L"-c") {
            searchEngine.clear();
            std::wcout << L"Directories cleared" << std::endl;
        } else if(input == L"-v") {
            searchEngine.verbose = !searchEngine.verbose;
            std::wcout << L"Verbose switched " << (searchEngine.verbose ? L"on." : L"off.") << std::endl;
        } else if(input == L"-q") {
            break;
        } else {
            std::wcout << std::quoted(input) << L" was not recognized\n";
        }
    }

    return 0;

}
