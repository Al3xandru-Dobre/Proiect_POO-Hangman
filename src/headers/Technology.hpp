//
// Created by Alex on 20.11.2024.
//

#ifndef TECHNOLOGY_HPP
#define TECHNOLOGY_HPP

#include "Word_Manager.hpp"

class TechnologyWordManager:public WordManager{
public:
    TechnologyWordManager();

    void selectRandomWordFromCategory() override;
};


#endif //TECHNOLOGY_HPP
