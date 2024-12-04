//
// Created by Alex on 20.11.2024.
//

#include "src/headers/Leaderboard.hpp"

    void Leaderboard::adaugaJucator(const std::string& nume) {
        listaScoruri.emplace_back(nume);
    }

    void Leaderboard::actualizeazaScor(const std::string& nume, int puncte) {
        for (auto& scor : listaScoruri) {
            if (scor.getNumeJucator() == nume) {
                scor.adaugaScor(puncte);
                return;
            }
        }

        listaScoruri.emplace_back(nume);
        listaScoruri.back().adaugaScor(puncte);
    }

    void Leaderboard::afiseazaLeaderboard() const {
        std::vector<Scor> scoruriOrdonate = listaScoruri;
        std::sort(scoruriOrdonate.begin(), scoruriOrdonate.end(), [](const Scor& a, const Scor& b) {
            return a.getScorCurent() > b.getScorCurent();
        });

        std::cout << "Leaderboard:\n";
        for (const auto& scor : scoruriOrdonate) {
            std::cout << scor.getNumeJucator() << ": " << scor.getScorCurent() << "\n";
        }
    }
