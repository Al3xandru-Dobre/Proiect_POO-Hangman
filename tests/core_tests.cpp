// Teste minimale pentru logica jocului (fara framework extern).
// Rulare: ctest --test-dir <build>   sau direct ./hangman_tests

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "headers/Game.hpp"
#include "headers/Game_Timer.hpp"
#include "headers/Hint_Game.hpp"
#include "headers/Leaderboard.hpp"
#include "headers/Word_Manager.hpp"

namespace {
int failures = 0;

#define CHECK(condition)                                                              \
    do {                                                                              \
        if (!(condition)) {                                                           \
            ++failures;                                                               \
            std::cerr << __FILE__ << ":" << __LINE__ << "  ESUAT: " #condition "\n";  \
        }                                                                             \
    } while (false)

// Un WordManager cu un cuvant fix: exemplu de extindere prin mostenire,
// fara a modifica nicio clasa existenta (principiul Open/Closed).
class FixedWordManager : public WordManager {
public:
    explicit FixedWordManager(std::string word) { wordMap["fixed"] = {std::move(word)}; }
    [[nodiscard]] std::string getCategoryKey() const override { return "fixed"; }
    [[nodiscard]] std::string getDisplayName() const override { return "Test"; }
    [[nodiscard]] std::string getDataFile() const override { return {}; }
};

Game makeGame(const std::string& word, int hints = 2, int timeLimit = 0) {
    Difficulty d = Difficulty::normal();
    d.hints = hints;
    d.timeLimitSeconds = timeLimit;
    return Game(std::make_unique<FixedWordManager>(word), std::make_unique<HintGame>(hints), d);
}

void testNormalizeWord() {
    CHECK(WordManager::normalizeWord("  River\r") == "river");
    CHECK(WordManager::normalizeWord("New York") == "new york");
    CHECK(WordManager::normalizeWord("   ").empty());
    CHECK(WordManager::normalizeWord("# comentariu").empty());
    CHECK(WordManager::normalizeWord("abc123").empty());
}

void testGuessing() {
    Game game = makeGame("mountain");
    CHECK(game.getGuessedWord() == "________");
    CHECK(game.guessLetter('M') == GuessResult::Correct); // majusculele sunt acceptate
    CHECK(game.getGuessedWord() == "m_______");
    CHECK(game.guessLetter('z') == GuessResult::Wrong);
    CHECK(game.getAttemptsRemaining() == 5);
    CHECK(game.guessLetter('z') == GuessResult::AlreadyGuessed); // repetarea nu costa
    CHECK(game.guessLetter('m') == GuessResult::AlreadyGuessed);
    CHECK(game.guessLetter('1') == GuessResult::Invalid);
    CHECK(game.getAttemptsRemaining() == 5);
    CHECK(game.guessWord("MOUNTAIN") == GuessResult::Correct);
    CHECK(game.isGameWon());
    CHECK(game.guessLetter('q') == GuessResult::GameFinished);
    CHECK(game.computeScore() > 0);
}

void testLosing() {
    Game game = makeGame("ab");
    for (char c : std::string("cdefgh")) {
        game.guessLetter(c);
    }
    CHECK(game.isGameLost());
    CHECK(game.isGameOver());
    CHECK(!game.isGameWon());
    CHECK(game.computeScore() == 0);
}

void testSpacesAreVisible() {
    Game game = makeGame("new york");
    CHECK(game.getGuessedWord() == "___ ____");
}

void testHints() {
    Game game = makeGame("aab", 2);
    const auto letter = game.useHint();
    CHECK(letter.has_value());
    CHECK(game.getAttemptsRemaining() == 6); // hint-ul nu consuma incercari
    CHECK(game.getHintGame().getHintsRemaining() == 1);
    CHECK(game.wasLetterTried(*letter));
    game.useHint();
    CHECK(game.isGameWon());              // doua litere distincte, doua hint-uri
    CHECK(!game.useHint().has_value());   // jocul s-a terminat
}

void testTimer() {
    GameTimer timer(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    CHECK(timer.isTimeUp());
    CHECK(timer.getTimeRemaining() == 0); // niciodata negativ
    CHECK(GameTimer(0).getRemainingFraction() == 1.0f);
}

void testLeaderboard() {
    const std::string path =
        (std::filesystem::temp_directory_path() / "hangman_test_leaderboard.txt").string();
    {
        Leaderboard board(path);
        board.actualizeazaScor("Ana", 50);
        board.actualizeazaScor("Ion", 80);
        board.actualizeazaScor("Ana", 40);
        CHECK(board.getTop(1).front().getNumeJucator() == "Ana");
        CHECK(board.salveaza());
    }
    Leaderboard reloaded(path);
    CHECK(reloaded.incarca());
    CHECK(reloaded.getNumarJucatori() == 2);
    CHECK(reloaded.getTop(2)[1].getScorCurent() == 80);
    std::remove(path.c_str());
}

void testFactoryAndFiles() {
    for (Category c : {Category::Animals, Category::Technology, Category::Geography, Category::Mixed}) {
        auto manager = createWordManager(c);
        manager->selectRandomWordFromCategory();
        CHECK(!manager->getSelectedWord().empty());
    }
}
} // namespace

int main() {
    testNormalizeWord();
    testGuessing();
    testLosing();
    testSpacesAreVisible();
    testHints();
    testTimer();
    testLeaderboard();
    testFactoryAndFiles();

    if (failures == 0) {
        std::cout << "Toate testele au trecut.\n";
        return 0;
    }
    std::cout << failures << " verificari au esuat.\n";
    return 1;
}
