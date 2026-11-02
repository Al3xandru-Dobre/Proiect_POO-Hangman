//
// Created by Alex on 20.11.2024.
//

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

// Gaseste un fisier de resurse (data/..., assets/...) indiferent de folderul
// din care este pornit jocul: intai relativ la directorul curent, apoi
// relativ la folderul proiectului (HANGMAN_RESOURCE_DIR, setat din CMake).
// Arunca std::runtime_error daca fisierul nu exista nicaieri.
std::string resolveResourcePath(const std::string& relativePath);

#endif //RESOURCES_HPP
