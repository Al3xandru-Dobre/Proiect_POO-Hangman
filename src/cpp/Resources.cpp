//
// Created by Alex on 20.11.2024.
//

#include "headers/Resources.hpp"

#include <filesystem>
#include <stdexcept>

#ifndef HANGMAN_RESOURCE_DIR
#define HANGMAN_RESOURCE_DIR "."
#endif

std::string resolveResourcePath(const std::string& relativePath) {
    namespace fs = std::filesystem;

    const fs::path candidates[] = {
        fs::path(relativePath),                                // langa executabil / directorul curent
        fs::path(HANGMAN_RESOURCE_DIR) / relativePath,         // folderul proiectului
        fs::path(relativePath).filename(),                     // compatibilitate: fisier in radacina
    };

    for (const auto& candidate : candidates) {
        std::error_code error;
        if (fs::is_regular_file(candidate, error)) {
            return candidate.string();
        }
    }
    throw std::runtime_error("Nu gasesc fisierul de resurse: " + relativePath);
}
