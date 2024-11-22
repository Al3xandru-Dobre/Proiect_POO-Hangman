//
// Created by Alex on 20.11.2024.
//

#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include "scor.hpp"
#include <algorithm>


class Leaderboard : public Scor {
private:
    std::vector<Scor> listaScoruri;

public:
    Leaderboard() : Scor("") {}

    void adaugaJucator(const std::string& nume) ;

    void actualizeazaScor(const std::string& nume, int puncte);

    void afiseazaLeaderboard() const ;
};



#endif //LEADERBOARD_HPP
