//
// Created by Alex on 08.12.2024.
//

#ifndef INVALIDIINPUTERORR_HPP
#define INVALIDIINPUTERORR_HPP

#include <exception>
#include <string>
#include <utility>

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

