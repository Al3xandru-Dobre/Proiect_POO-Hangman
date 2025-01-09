//
// Created by Alex on 07.01.2025.
//

#ifndef WORDMANAGERFACOTRY_HPP
#define WORDMANAGERFACOTRY_HPP
#include <functional>
#include <memory>
#include "Word_Manager.hpp"
#include "Animal.hpp"
#include "Geografie.hpp"
#include "Technology.hpp"
/*
 *Aceasta clasa este un factory pentru WordManager
 *in functie de alegerea utilizatorului, se creeaza un WordManager diferit
 *arunca o exceptie care duce la assignarea clasei deafult in cazul in care alegerea nu este valida, tratat in manager
 */
class WordManagerFacotry {
public:
    static std::unique_ptr<WordManager> createWordManager(int choice) {
        switch (choice) {
            case 1:
                return std::make_unique<AnimalWordManager>();
            case 2:
                return std::make_unique<TechnologyWordManager>();
            case 3:
                return std::make_unique<GeographyWordManager>();
            default:
                throw InvalidInputException("Categorie invalidă! Trebuie să introduci un număr între 1 și 3.");
        }
    }
};



#endif //WORDMANAGERFACOTRY_HPP
