//
// Created by Alex on 20.11.2024.
//

#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

// Interfata abstracta (in sens OOP) pentru orice mod de afisare a jocului.
// main() lucreaza doar cu un std::unique_ptr<UserInterface> si apeleaza run();
// legarea dinamica decide daca ruleaza ConsoleUI sau HangmanUI (SFML).
class UserInterface {
public:
    virtual ~UserInterface() = default;
    virtual int run() = 0; // intoarce codul de iesire al programului
};

#endif //USER_INTERFACE_HPP
