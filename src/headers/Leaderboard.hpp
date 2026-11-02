//
// Created by Alex on 20.11.2024.
//

#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <string>
#include <vector>

#include "scor.hpp"

// Clasamentul NU este un scor ("is-a"), ci contine scoruri ("has-a").
// De aceea foloseste compozitie (std::vector<Scor>) in loc de mostenire.
class Leaderboard {
private:
    std::vector<Scor> listaScoruri;
    std::string caleFisier;

public:
    explicit Leaderboard(std::string fisier = "leaderboard.txt");

    void adaugaJucator(const std::string& nume);
    void actualizeazaScor(const std::string& nume, int puncte);

    [[nodiscard]] std::vector<Scor> getTop(std::size_t n) const; // ordonat descrescator
    [[nodiscard]] std::size_t getNumarJucatori() const;
    [[nodiscard]] int getScorJucator(const std::string& nume) const; // 0 daca nu exista

    void afiseazaLeaderboard() const;

    bool incarca();       // citeste din fisier (format: nume;puncte)
    bool salveaza() const;
};

#endif //LEADERBOARD_HPP
