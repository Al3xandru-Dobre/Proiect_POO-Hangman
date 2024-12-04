#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include "scor.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class Leaderboard {
private:
    std::vector<Scor> listaScoruri;
    std::string fisierScoruri;

    void incarcaScoruriDinFisier();

public:
    explicit Leaderboard(const std::string& numeFisier);
    void adaugaJucator(const std::string& nume);
    void actualizeazaScor(const std::string& nume, int puncte);
    void afiseazaLeaderboard() const;

    void salveazaScoruriInFisier() const;
    [[nodiscard]] int obtineScorMaxim() const;
};

#endif // LEADERBOARD_HPP
