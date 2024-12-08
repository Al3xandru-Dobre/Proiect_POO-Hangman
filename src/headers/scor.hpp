//
// Created by Alex on 20.11.2024.
//

#ifndef SCOR_HPP
#define SCOR_HPP

#include <string>
#include "Leaderboard.hpp"
class Leaderboard;

class Scor {
protected:
    int scorCurent = 0;
    std::string numeJucator;
public:
    Scor(const std::string& nume = "Anonim");

    void adaugaScor(int puncte);

    [[nodiscard]] int getScorCurent() const ;

    [[nodiscard]] std::string getNumeJucator() const;

    void trimiteScorLaLeaderboard(Leaderboard& leaderboard) const;

    void afiseazaScor() const;
    virtual ~Scor() = default;
};



#endif //SCOR_HPP
