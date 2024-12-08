#include "Leaderboard.hpp"

// Constructor - încarcă scorurile din fișier
Leaderboard::Leaderboard(const std::string& numeFisier) : fisierScoruri(numeFisier) {
    incarcaScoruriDinFisier();
}

// Metodă privată: Încarcă scorurile din fișier
void Leaderboard::incarcaScoruriDinFisier() {
    std::ifstream fisier(fisierScoruri);
    if (!fisier.is_open()) {
        std::cerr << "Nu s-a putut deschide fișierul: " << fisierScoruri << "\n";
        return;
    }

    std::string nume;
    int scor;
    while (fisier >> nume >> scor) {
        listaScoruri[nume] = Scor(nume);
        listaScoruri[nume].adaugaScor(scor);
    }
    fisier.close();
}

// Metodă publică: Salvează scorurile în fișier
void Leaderboard::salveazaScoruriInFisier() const {
    std::ofstream fisier(fisierScoruri, std::ios::trunc);
    if (!fisier.is_open()) {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: " << fisierScoruri << "\n";
        exit(1);
    }
    for(const auto& [nume, scor] : listaScoruri) {
        fisier << nume << " " << scor.getScorCurent() << "\n";
    }
    fisier.close();
}

// Metodă publică: Obține scorul maxim
int Leaderboard::obtineScorMaxim() const {
    int maxScor = 0;
    for(const auto& [nume, scor] : listaScoruri) {
        maxScor = std::max(maxScor, scor.getScorCurent());
    }
    return maxScor;
}

void Leaderboard::adaugaJucator(const std::string& nume) {
    if (listaScoruri.find(nume) != listaScoruri.end()) {
        std::cout << "Jucătorul " << nume << " există deja în leaderboard.\n";
    } else {
        listaScoruri[nume] = Scor(nume);
        std::cout << "Jucătorul " << nume << " a fost adăugat în leaderboard.\n";
    }
}

void Leaderboard::actualizeazaScor(const std::string& nume, int puncte) {
    listaScoruri[nume].adaugaScor(puncte);
}

// Afișează leaderboard-ul ordonat
void Leaderboard::afiseazaLeaderboard() const {
    if (listaScoruri.empty()) {
        std::cout << "Leaderboard-ul este gol.\n";
        return;
    }
    std::vector<std::pair<std::string, int>> scoruri;
    for(const auto& [nume,scor] : listaScoruri) {
        scoruri.emplace_back(nume, scor.getScorCurent());
    }

    //Sortare descrescatoare după scor
    std::sort(scoruri.begin(), scoruri.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });

    std::cout << "Leaderboard:\n";
    for (const auto& [nume, scor] : scoruri) {
        std::cout << nume << ": " << scor << "\n";
    }
}

int Leaderboard::obtineScorJucator(const std::string &nume) const {
    auto it = listaScoruri.find(nume);
    return (it != listaScoruri.end()) ? it->second.getScorCurent() : 0;
}
