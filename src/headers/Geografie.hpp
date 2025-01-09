//
// Created by Alex on 09.01.2025.
//

#ifndef GEOGRAFIE_HPP
#define GEOGRAFIE_HPP

#include "Word_Manager.hpp"

class GeographyWordManager:public WordManager{
public:
    GeographyWordManager();
    void selectRandomWordFromCategory() override;
};
#endif //GEOGRAFIE_HPP
