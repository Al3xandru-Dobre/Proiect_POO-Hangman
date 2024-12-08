//
// Created by Alex on 08.12.2024.
//

#include <iostream>
#include <limits>
#include "invalidInputException.hpp"

int citesteCategorie() {
    int categorie;
    std::cout << "Introdu numarul categoriei: ";

    // Verifică inputul
    if (!(std::cin >> categorie)) {
        throw InvalidInputException("Input invalid! Trebuie să introduci un număr.");
    }

    return categorie;
}
