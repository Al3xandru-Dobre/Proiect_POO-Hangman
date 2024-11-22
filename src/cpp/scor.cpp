//
// Created by Alex on 20.11.2024.
//

#include "headers/scor.hpp"
#include <iostream>


Scor::Scor(const std::string& nume) : numeJucator(nume), scorCurent(0) {}

void Scor::adaugaScor(int puncte) {
    scorCurent += puncte;
}

int Scor::getScorCurent() const {
    return scorCurent;
}

std::string Scor::getNumeJucator() const  {
    return numeJucator;
}

void Scor ::afiseazaScor() const  {
    std::cout << "Scorul jucatorului " << numeJucator << ": " << scorCurent << "\n";
}