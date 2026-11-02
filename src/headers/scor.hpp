//
// Created by Alex on 20.11.2024.
//

#ifndef SCOR_HPP
#define SCOR_HPP

#include <iosfwd>
#include <string>

class Scor {
protected:
    // Ordinea declararii coincide cu ordinea din lista de initializare.
    std::string numeJucator;
    int scorCurent;

public:
    explicit Scor(const std::string& nume, int puncte = 0);
    virtual ~Scor() = default;

    void adaugaScor(int puncte);

    [[nodiscard]] int getScorCurent() const;
    [[nodiscard]] const std::string& getNumeJucator() const;

    void afiseazaScor() const;

    // Supraincarcarea operatorilor: sortare naturala si afisare in flux.
    bool operator>(const Scor& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Scor& scor);
};

#endif //SCOR_HPP
