//
// Created by Alex on 08.12.2024.
//

#ifndef INVALIDIINPUTERORR_HPP
#define INVALIDIINPUTERORR_HPP

#include <exception>
#include <string>
#include <utility>
/*
 *Aceasta clasa este folosita pentru a genera exceptii personalizate
 *Este de mare ajutor mai ales cand ceva nu merge sau nu s-a introdus corect / generat bine, deoarece permite remedieri si tratarea cazuri speciale
 */

class InvalidInputException : public std::exception {
private:
    std::string mesaj;
public:
    explicit InvalidInputException(std::string  msg) : mesaj(std::move(msg)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

#endif //INVALIDIINPUTERORR_HPP

