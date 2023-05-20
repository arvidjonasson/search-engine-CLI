//
// Created by Arvid Jonasson on 2023-05-19.
//

#ifndef SEARCH_ENGINE_DIRECTORYMANAGER_H
#define SEARCH_ENGINE_DIRECTORYMANAGER_H
#include <filesystem>
#include <vector>
#include <unordered_set>

class DirectoryManager {
public:
    DirectoryManager addDirectory(const std::filesystem::path &directory_path);
    [[nodiscard]] std::vector<std::filesystem::path> getFiles();
    [[nodiscard]] std::vector<std::filesystem::path> getDirectories() const;

    void clear() noexcept;

private:
    std::vector<std::filesystem::path> directory_paths;
    // A vector of the directories that have been searched
    std::unordered_set<std::wstring> included;
    // A set to keep track of visited directiories, thus avoid including a file twice
    std::vector<std::filesystem::path> files;
    // Files that will be returned to the user
};


#endif //SEARCH_ENGINE_DIRECTORYMANAGER_H
