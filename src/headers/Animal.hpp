//
// Created by Alex on 09.01.2025.
//

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include "Word_Manager.hpp"

class AnimalWordManager:public WordManager{
public:
    AnimalWordManager();

    void selectRandomWordFromCategory() override;
};



#endif //ANIMAL_HPP
