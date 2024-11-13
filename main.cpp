#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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

class GameTimer {
private:
    std::chrono::steady_clock::time_point startTime;
    int timeLimit;

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
protected:
    WordManager wordManager;
    std::string guessedWord;
    int attemptsRemaining;

public:
    Game() : attemptsRemaining(6) {
        wordManager.selectRandomWord();
        guessedWord = std::string(wordManager.getSelectedWord().size(), '_');
    }

    bool guessLetter(char letter) {
        bool found = false;
        for (size_t i = 0; i < wordManager.getSelectedWord().size(); i++) {
            if (wordManager.getSelectedWord()[i] == letter) {
                guessedWord[i] = letter;
                found = true;
            }
        }
        if (!found) {
            attemptsRemaining--;
        }
        return found;
    }

    bool guessWord(const std::string& word) {
        if (word == wordManager.getSelectedWord()) {
            guessedWord = word;
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

    std::string getSelectedWord() const {  // Added this getter method
        return wordManager.getSelectedWord();
    }

    void displayStatus() const {
        std::cout << "Cuvant: " << guessedWord << "\n";
        std::cout << "Incercari ramase: " << attemptsRemaining << "\n";
    }
};

class HintGame : public Game {
private:
    std::vector<std::string> hints;
    int hintsAvailable;

public:
    HintGame(int maxHints) : hintsAvailable(maxHints) {}

    void setHints(const std::vector<std::string>& hintList) {
        hints = hintList;
    }

    void provideHint() {
        if (hintsAvailable > 0 && !hints.empty()) {
            std::cout << "Hint: " << hints.back() << "\n";
            hints.pop_back();
            hintsAvailable--;
        } else {
            std::cout << "Nu mai sunt hint-uri disponibile.\n";
        }
    }

    bool requestHintOnIncorrectGuess() {
        if (hintsAvailable > 0) {
            provideHint();
            return true;
        }
        return false;
    }
};

class HangmanUI {
    HintGame game;

public:
    HangmanUI() : game(3) {
        game.setHints({"Primul hint", "Al doilea hint", "Al treilea hint"});
    }

    void start() {
        std::string input;
        std::cout << "Bine ai venit la jocul de Spanzuratoarea!\n";

        while (!game.isGameOver() && !game.isGameWon()) {
            game.displayStatus();
            std::cout << "Introdu o litera sau un cuvant complet: ";
            std::cin >> input;

            bool correctGuess;
            if (input.size() == 1) { // Este o literă
                char letter = input[0];
                correctGuess = game.guessLetter(letter);
            } else { // Este un cuvânt întreg
                correctGuess = game.guessWord(input);
            }

            if (correctGuess) {
                std::cout << "Ai ghicit corect!\n";
            } else {
                std::cout << "Nu ai ghicit corect.\n";
                game.requestHintOnIncorrectGuess(); // Oferă hint după un răspuns greșit, dacă e disponibil
            }
        }

        if (game.isGameWon()) {
            std::cout << "Felicitari! Ai ghicit cuvantul: " << game.getGuessedWord() << "\n";
        } else {
            std::cout << "Ai pierdut! Cuvantul era: " << game.getSelectedWord() << "\n"; // Using the getter method
        }
    }

    bool gameWon() const {
        return game.isGameWon();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    HangmanUI ui;
    Leaderboard leaderboard;
    GameTimer timer(300);

    std::string numeJucator;
    std::cout << "Introduceti numele jucătorului: ";
    std::cin >> numeJucator;

    leaderboard.adaugaJucator(numeJucator);

    while (!timer.isTimeUp()) {
        ui.start();

        if (ui.gameWon()) {
            leaderboard.actualizeazaScor(numeJucator, 10);
            break;
        } else {
            std::cout << "Încercați din nou!\n";
        }

        std::cout << "Timp ramas: " << timer.getTimeRemaining() << " secunde\n";

        leaderboard.afiseazaLeaderboard();
    }

    std::cout << "Timpul a expirat! Jocul s-a încheiat.\n";
    leaderboard.afiseazaLeaderboard();

    return 0;
}
