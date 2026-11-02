//
// Created by Alex on 20.11.2024.
//

#ifndef HINT_GAME_HPP
#define HINT_GAME_HPP

#include <optional>
#include <string>

// Gestioneaza hint-urile: un hint dezvaluie o litera aleatorie care nu a fost
// inca ghicita. Hint-ul se cere explicit de jucator si NU consuma o incercare.
class HintGame {
protected:
    int hintsAvailable; // Numarul de hint-uri ramase
    int hintsUsed = 0;  // Cate hint-uri a folosit jucatorul (penalizare la scor)

public:
    explicit HintGame(int maxHints = 3);
    virtual ~HintGame() = default;

    // Alege o litera din `secret` aflata pe o pozitie inca ascunsa ('_') in `progress`.
    [[nodiscard]] std::optional<char> getRandomUnrevealedLetter(const std::string& secret,
                                                                const std::string& progress) const;

    // Consuma un hint si intoarce litera de dezvaluit (sau nimic daca nu se poate).
    // Virtuala: o clasa derivata poate schimba strategia (ex. litera cea mai frecventa).
    virtual std::optional<char> provideHint(const std::string& secret, const std::string& progress);

    [[nodiscard]] bool canProvideHint() const;
    [[nodiscard]] int getHintsRemaining() const;
    [[nodiscard]] int getHintsUsed() const;
};

#endif //HINT_GAME_HPP
