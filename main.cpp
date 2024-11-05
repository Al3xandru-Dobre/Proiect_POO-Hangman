#include <iostream>
<<<<<<< HEAD
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>

class Scor {
protected:
    int scorCurent;
    std::string numeJucator;

public:
    Scor(const std::string& nume) : numeJucator(nume), scorCurent(0) {}

    void adaugaScor(int puncte) {
        scorCurent += puncte;
    }

    int getScorCurent() const {
        return scorCurent;
    }

    std::string getNumeJucator() const {
        return numeJucator;
    }

    void afiseazaScor() const {
        std::cout << "Scorul jucatorului " << numeJucator << ": " << scorCurent << "\n";
    }
};


class Leaderboard : public Scor {
private:
    std::vector<Scor> listaScoruri;

public:
    Leaderboard() : Scor("") {}

    void adaugaJucator(const std::string& nume) {
        listaScoruri.emplace_back(nume);
    }

    void actualizeazaScor(const std::string& nume, int puncte) {
        for (auto& scor : listaScoruri) {
            if (scor.getNumeJucator() == nume) {
                scor.adaugaScor(puncte);
                return;
            }
        }

        listaScoruri.emplace_back(nume);
        listaScoruri.back().adaugaScor(puncte);
    }

    void afiseazaLeaderboard() const {

        std::vector<Scor> scoruriOrdonate = listaScoruri;
        std::sort(scoruriOrdonate.begin(), scoruriOrdonate.end(), [](const Scor& a, const Scor& b) {
            return a.getScorCurent() > b.getScorCurent();
        });

        std::cout << "Leaderboard:\n";
        for (const auto& scor : scoruriOrdonate) {
            std::cout << scor.getNumeJucator() << ": " << scor.getScorCurent() << "\n";
        }
    }
};

class HintSystem {
private:
    std::vector<std::string> hints;
    int hintsAvailable;
    std::chrono::steady_clock::time_point lastHintTime;
    int hintInterval; // in seconds

public:
    HintSystem(int maxHints, int interval) : hintsAvailable(maxHints), hintInterval(interval) {}

    void setHints(const std::vector<std::string>& hintList) {
        hints = hintList;
    }

    void provideHint() {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastHintTime).count();

        if (hintsAvailable > 0 && elapsedTime >= hintInterval) {
            std::cout << "Hint: " << hints.back() << "\n";
            hints.pop_back();
            hintsAvailable--;
            lastHintTime = currentTime;
        } else {
            std::cout << "Hint not available yet.\n";
        }
    }

    int getHintsAvailable() const {
        return hintsAvailable;
    }
};

class GameTimer {
private:
    std::chrono::steady_clock::time_point startTime;
    int timeLimit; // în secunde

public:
    GameTimer(int limit) : timeLimit(limit) {
        startTime = std::chrono::steady_clock::now();
    }

    bool isTimeUp() const {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        return elapsed >= timeLimit;
    }

    int getTimeRemaining() const {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        return timeLimit - elapsed;
    }
};

class WordManager {
private:
    std::vector<std::string> wordList;
    std::string selectedWord;

public:
    WordManager() {
        wordList = {"programare", "calculator", "cuvant", "proiect", "spanzuratoare"};
    }

    void selectRandomWord() {
        srand(static_cast<unsigned int>(time(0)));
        int index = rand() % wordList.size();
        selectedWord = wordList[index];
    }

    std::string getSelectedWord() const {
        return selectedWord;
    }

    bool isLetterInWord(char letter) const {
        return selectedWord.find(letter) != std::string::npos;
    }
};

class Game {
private:
    WordManager wordManager;
    std::string guessedWord;
    int attemptsRemaining;
    std::vector<char> guessedLetters;

public:
    Game() : attemptsRemaining(6) {
        wordManager.selectRandomWord();
        guessedWord = std::string(wordManager.getSelectedWord().size(), '_');
    }

    bool guessLetter(char letter) {
        if (wordManager.isLetterInWord(letter)) {
            for (size_t i = 0; i < wordManager.getSelectedWord().size(); i++) {
                if (wordManager.getSelectedWord()[i] == letter) {
                    guessedWord[i] = letter;
                }
            }
            return true;
        } else {
            attemptsRemaining--;
            return false;
        }
    }

    bool isGameWon() const {
        return guessedWord == wordManager.getSelectedWord();
    }

    bool isGameOver() const {
        return attemptsRemaining <= 0;
    }

    int getAttemptsRemaining() const {
        return attemptsRemaining;
    }

    std::string getGuessedWord() const {
        return guessedWord;
    }

    void displayStatus() const {
        std::cout << "Cuvant: " << guessedWord << "\n";
        std::cout << "Incercari ramase: " << attemptsRemaining << "\n";
    }
    friend class HangmanUI;
};

class HangmanUI {
    Game game;

public:
    void start() {
        char letter;
        std::cout << "Bine ai venit la jocul de Spânzurătoarea!\n";

        while (!game.isGameOver() && !game.isGameWon()) {
            game.displayStatus();
            std::cout << "Introdu o litera: ";
            std::cin >> letter;
            if (game.guessLetter(letter)) {
                std::cout << "Litera " << letter << " se afla in cuvant!\n";
            } else {
                std::cout << "Litera " << letter << " NU se afla in cuvant.\n";
            }
        }

        if (game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << game.getGuessedWord() << "\n";
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << game.getGuessedWord() << "\n";
        }
    }

    bool gameWon() const {
        return game.isGameWon();
    }
};

int main() {
    HangmanUI ui;
    Leaderboard leaderboard;
    HintSystem hintSystem(3, 30);
    hintSystem.setHints({"Primul hint", "Al doilea hint", "Al treilea hint"});
    GameTimer timer(300);

    std::string numeJucator;
    std::cout << "Introduceti numele jucătorului: ";
    std::cin >> numeJucator;

    leaderboard.adaugaJucator(numeJucator);

    while (!timer.isTimeUp() && !ui.gameWon()) {
        ui.start();

        if (ui.gameWon()) {
            leaderboard.actualizeazaScor(numeJucator, 10);
            break;
        } else {
            std::cout << "Încercați din nou!\n";
        }

        std::cout << "Timp ramas: " << timer.getTimeRemaining() << " secunde\n";

        hintSystem.provideHint();

        leaderboard.afiseazaLeaderboard();
    }

    std::cout << "Timpul a expirat! Jocul s-a încheiat.\n";
    leaderboard.afiseazaLeaderboard();

    return 0;
}




=======
#include <array>

#include <Helper.h>

int main() {
    std::cout << "Hello, world!\n";
    std::array<int, 100> v{};
    int nr;
    std::cout << "Introduceți nr: ";
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////
    std::cin >> nr;
    /////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < nr; ++i) {
        std::cout << "v[" << i << "] = ";
        std::cin >> v[i];
    }
    std::cout << "\n\n";
    std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
    for(int i = 0; i < nr; ++i) {
        std::cout << "- " << v[i] << "\n";
    }
    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////
    ///                Exemplu de utilizare cod generat                     ///
    ///////////////////////////////////////////////////////////////////////////
    Helper helper;
    helper.help();
    ///////////////////////////////////////////////////////////////////////////
    return 0;
}
>>>>>>> origin/main
