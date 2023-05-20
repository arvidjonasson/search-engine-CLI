//
// Created by Arvid Jonasson on 2023-05-19.
//

#include "DirectoryManager.h"

std::vector<std::filesystem::path> DirectoryManager::getFiles() {
    std::vector<std::filesystem::path> tmp(std::move(files));
    files.clear();
    return tmp;
    // Returns the files that have yet not been added to the tf-idf calculation
    // Resets files to an empty vector
}

DirectoryManager DirectoryManager::addDirectory(const std::filesystem::path &directory_path) {
    std::wstring absolute_directory_path(std::filesystem::canonical(directory_path).wstring());
    if(included.contains(absolute_directory_path))
        return *this;

    included.insert(absolute_directory_path);
    directory_paths.push_back(directory_path);

    std::vector<std::filesystem::path> new_files;
    for(std::filesystem::recursive_directory_iterator iterator
    (directory_path,std::filesystem::directory_options::skip_permission_denied);
    iterator != std::filesystem::end(iterator); ++iterator) {
        auto &entry = *iterator;
        if(entry.is_directory()) {
            std::wstring absolute_path(std::filesystem::canonical(entry.path()).wstring());
            if(included.contains(absolute_path))
                iterator.disable_recursion_pending();
            else
                included.insert(absolute_path);
            // If the directory is already included, we don't want to recurse into it.
            // Else we will mark it as already included
        } else if(entry.is_regular_file() && !entry.path().filename().string().starts_with('.')) {
            new_files.push_back(entry.path());
        }
    }

    files.insert(files.end(), new_files.begin(), new_files.end());

    return *this;
}

std::vector<std::filesystem::path> DirectoryManager::getDirectories() const {
    return directory_paths;
}

void DirectoryManager::clear() noexcept {
    files           .clear();
    included        .clear();
    directory_paths .clear();
}
