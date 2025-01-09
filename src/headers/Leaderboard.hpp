#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include "scor.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

class Scor;
/*
 *Aceasta clasa are ca scop de a pastra scorurile jucatorilor, si de a le actualiza
 *Scorurile sunt primite de la joc si sunt pastrate in memorie, intr-un fisier care se updateaza la fiecare joc
 *Exista si metode de resetare a scorurilor, de afisare a scorurilor si de adaugare a unui jucator, cat si de gasire a unui jucator
 *
 */

class Leaderboard{
private:
    std::unordered_map<std::string, Scor>listaScoruri;
    std::string fisierScoruri;

    void incarcaScoruriDinFisier();

public:
    explicit Leaderboard(const std::string& numeFisier);
    void adaugaJucator(const std::string& nume);
    void actualizeazaScor(const std::string& nume, int puncte);
    void afiseazaLeaderboard() const;
    void salveazaScoruriInFisier() const;
    [[nodiscard]] int obtineScorMaxim() const;
    bool existaJucator(const std::string& nume) const;
    void reseteazaLeaderboard();
    int obtineScorJucator(const std::string& nume) const;

    ~Leaderboard() = default;
};

#endif // LEADERBOARD_HPP
