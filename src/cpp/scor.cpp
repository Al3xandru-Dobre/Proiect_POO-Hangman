//
// Created by Alex on 20.11.2024.
//

#include "headers/scor.hpp"

#include <iostream>

Scor::Scor(const std::string& nume, int puncte) : numeJucator(nume), scorCurent(puncte) {}

void Scor::adaugaScor(int puncte) {
    scorCurent += puncte;
}

int Scor::getScorCurent() const {
    return scorCurent;
}

const std::string& Scor::getNumeJucator() const {
    return numeJucator;
}

void Scor::afiseazaScor() const {
    std::cout << *this << "\n";
}

bool Scor::operator>(const Scor& other) const {
    return scorCurent > other.scorCurent;
}

std::ostream& operator<<(std::ostream& out, const Scor& scor) {
    return out << scor.numeJucator << ": " << scor.scorCurent;
}
