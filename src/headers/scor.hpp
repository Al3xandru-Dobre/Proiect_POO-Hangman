//
// Created by Alex on 20.11.2024.
//

#ifndef SCOR_HPP
#define SCOR_HPP

#include <string>

class Scor {
protected:
    int scorCurent;
    std::string numeJucator;
public:
    Scor(const std::string& nume);

    void adaugaScor(int puncte);

    [[nodiscard]] int getScorCurent() const ;

    [[nodiscard]] std::string getNumeJucator() const;

    void afiseazaScor() const;
};



#endif //SCOR_HPP
