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
        Scor jucator(nume);
        jucator.adaugaScor(scor);
        listaScoruri.emplace_back(jucator);
    }

    fisier.close();
}

// Metodă publică: Salvează scorurile în fișier
void Leaderboard::salveazaScoruriInFisier() const {
    std::ofstream fisier(fisierScoruri, std::ios::trunc);
    if (!fisier.is_open()) {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: " << fisierScoruri << "\n";
        return;
    }

    for (const auto& scor : listaScoruri) {
        fisier << scor.getNumeJucator() << " " << scor.getScorCurent() << "\n";
    }

    fisier.close();
}

// Metodă publică: Obține scorul maxim
int Leaderboard::obtineScorMaxim() const {
    if (listaScoruri.empty()) {
        return 0;
    }

    return std::max_element(listaScoruri.begin(), listaScoruri.end(), [](const Scor& a, const Scor& b) {
        return a.getScorCurent() < b.getScorCurent();
    })->getScorCurent();
}

void Leaderboard::adaugaJucator(const std::string& nume) {
    // Verifică dacă jucătorul există deja
    for (const auto& scor : listaScoruri) {
        if (scor.getNumeJucator() == nume) {
            std::cout << "Jucătorul " << nume << " există deja pe leaderboard.\n";
            return;
        }
    }
    // Adaugă un nou jucător cu scorul inițial 0
    listaScoruri.emplace_back(nume);
    std::cout << "Jucătorul " << nume << " a fost adăugat pe leaderboard.\n";
}

void Leaderboard::actualizeazaScor(const std::string& nume, int puncte) {
    for (auto& scor : listaScoruri) {
        if (scor.getNumeJucator() == nume) {
            scor.adaugaScor(puncte); // Adaugă punctele la scorul curent
            std::cout << "Scorul pentru " << nume << " a fost actualizat la " << scor.getScorCurent() << ".\n";
            return;
        }
    }

    // Dacă jucătorul nu există, îl adaugăm și îi atribuim punctele
    listaScoruri.emplace_back(nume);
    listaScoruri.back().adaugaScor(puncte);
    std::cout << "Jucătorul " << nume << " a fost adăugat cu scorul inițial de " << puncte << ".\n";
}

// Afișează leaderboard-ul ordonat
void Leaderboard::afiseazaLeaderboard() const {
    if (listaScoruri.empty()) {
        std::cout << "Leaderboard-ul este gol.\n";
        return;
    }

    // Creează o copie a listei pentru a o sorta
    std::vector<Scor> scoruriOrdonate = listaScoruri;
    std::sort(scoruriOrdonate.begin(), scoruriOrdonate.end(), [](const Scor& a, const Scor& b) {
        return a.getScorCurent() > b.getScorCurent(); // Sortare descrescătoare după scor
    });

    std::cout << "Leaderboard:\n";
    for (const auto& scor : scoruriOrdonate) {
        std::cout << scor.getNumeJucator() << ": " << scor.getScorCurent() << "\n";
    }
}
