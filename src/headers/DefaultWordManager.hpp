//
// Created by Alex on 09.01.2025.
//

#ifndef DEFAULTWORDMANAGER_HPP
#define DEFAULTWORDMANAGER_HPP

#include "Word_Manager.hpp"

class DefaultWordManager : public WordManager {
public:
    DefaultWordManager() = default;
    void selectRandomWordFromCategory() override;
};



#endif //DEFAULTWORDMANAGER_HPP
