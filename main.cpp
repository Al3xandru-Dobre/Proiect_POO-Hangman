#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>

#include "headers/Console_UI.hpp"
#include "headers/User_Interface.hpp"

#ifdef HANGMAN_WITH_GUI
#include "headers/Hangman_UI.hpp"
#endif

// Porneste interfata grafica implicit; cu argumentul --console porneste varianta text.
int main(int argc, char* argv[]) {
    [[maybe_unused]] bool consoleMode = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--console") == 0) {
            consoleMode = true;
        }
    }

#if defined(HANGMAN_WITH_GUI) && defined(__linux__)
    // Fara server grafic, SFML apeleaza std::abort() (nu arunca exceptie),
    // asa ca verificam inainte si trecem pe varianta text.
    if (!consoleMode && !std::getenv("DISPLAY")) {
        std::cerr << "Nu exista afisaj grafic (DISPLAY); pornesc varianta text.\n";
        consoleMode = true;
    }
#endif

    try {
        std::unique_ptr<UserInterface> ui; // polimorfism: tipul concret se alege la rulare
#ifdef HANGMAN_WITH_GUI
        if (!consoleMode) {
            ui = std::make_unique<HangmanUI>();
        }
#endif
        if (!ui) {
            ui = std::make_unique<ConsoleUI>();
        }
        return ui->run();
    } catch (const std::exception& ex) {
        std::cerr << "Eroare: " << ex.what() << "\n";
        return 1;
    }
}
