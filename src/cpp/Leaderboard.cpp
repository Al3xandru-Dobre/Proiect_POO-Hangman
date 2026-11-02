//
// Created by Alex on 20.11.2024.
//

#include "headers/Leaderboard.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
// Numele nu poate contine separatorul ';' sau caractere de linie noua.
std::string curataNume(const std::string& nume) {
    std::string rezultat;
    for (char c : nume) {
        if (c != ';' && c != '\n' && c != '\r') {
            rezultat += c;
        }
    }
    return rezultat.empty() ? std::string("Anonim") : rezultat;
}
} // namespace

Leaderboard::Leaderboard(std::string fisier) : caleFisier(std::move(fisier)) {}

void Leaderboard::adaugaJucator(const std::string& nume) {
    actualizeazaScor(nume, 0);
}

void Leaderboard::actualizeazaScor(const std::string& nume, int puncte) {
    const std::string numeCurat = curataNume(nume);
    for (auto& scor : listaScoruri) {
        if (scor.getNumeJucator() == numeCurat) {
            scor.adaugaScor(puncte);
            return;
        }
    }
    listaScoruri.emplace_back(numeCurat, puncte);
}

std::vector<Scor> Leaderboard::getTop(std::size_t n) const {
    std::vector<Scor> ordonate = listaScoruri;
    // stable_sort: la egalitate se pastreaza ordinea in care au aparut jucatorii
    std::stable_sort(ordonate.begin(), ordonate.end(), [](const Scor& a, const Scor& b) { return a > b; });
    if (ordonate.size() > n) {
        ordonate.erase(ordonate.begin() + static_cast<std::ptrdiff_t>(n), ordonate.end());
    }
    return ordonate;
}

std::size_t Leaderboard::getNumarJucatori() const {
    return listaScoruri.size();
}

int Leaderboard::getScorJucator(const std::string& nume) const {
    const std::string numeCurat = curataNume(nume);
    for (const auto& scor : listaScoruri) {
        if (scor.getNumeJucator() == numeCurat) {
            return scor.getScorCurent();
        }
    }
    return 0;
}

void Leaderboard::afiseazaLeaderboard() const {
    std::cout << "Leaderboard:\n";
    int loc = 1;
    for (const auto& scor : getTop(listaScoruri.size())) {
        std::cout << "  " << loc++ << ". " << scor << "\n";
    }
}

bool Leaderboard::incarca() {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        return false; // la prima rulare fisierul nu exista inca
    }
    listaScoruri.clear();
    std::string linie;
    while (std::getline(fisier, linie)) {
        if (!linie.empty() && linie.back() == '\r') {
            linie.pop_back();
        }
        const auto separator = linie.rfind(';');
        if (separator == std::string::npos) {
            continue;
        }
        std::istringstream puncteText(linie.substr(separator + 1));
        int puncte = 0;
        if (puncteText >> puncte) {
            actualizeazaScor(linie.substr(0, separator), puncte);
        }
    }
    return true;
}

bool Leaderboard::salveaza() const {
    std::ofstream fisier(caleFisier, std::ios::trunc);
    if (!fisier.is_open()) {
        return false;
    }
    for (const auto& scor : listaScoruri) {
        fisier << scor.getNumeJucator() << ';' << scor.getScorCurent() << '\n';
    }
    return static_cast<bool>(fisier);
}
