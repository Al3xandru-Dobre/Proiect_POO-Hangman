#include "HangmanGameManager.hpp"

int main() {
    try {
        auto manager = std::make_unique<HangmanGameManager>();// Inițializează managerul jocului
        manager->startJoc();         // Pornește jocul
    } catch (const std::exception& e) {
        std::cerr << "Eroare critica: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
