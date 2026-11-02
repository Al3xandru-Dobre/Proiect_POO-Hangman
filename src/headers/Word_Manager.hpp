//
// Created by Alex on 20.11.2024.
//

#ifndef WORD_MANAGER_HPP
#define WORD_MANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Clasa abstracta de baza pentru categoriile de cuvinte.
//
// selectRandomWordFromCategory() este o "metoda sablon" (Template Method):
// algoritmul (incarca o singura data -> verifica -> alege aleatoriu) este fix
// in clasa de baza, iar clasele derivate furnizeaza doar detaliile prin
// metodele virtuale getCategoryKey(), getDataFile() si, optional, loadWords().
class WordManager {
protected:
    std::unordered_map<std::string, std::vector<std::string>> wordMap; // Hash table: categorie -> cuvinte
    std::string selectedWord;

    // Citeste un fisier, normalizeaza fiecare linie si o adauga in wordMap[category].
    void loadWordsFromFile(const std::string& fileName, const std::string& category);

    // Punct de extensie: implicit incarca getDataFile() in getCategoryKey().
    // DefaultWordManager il suprascrie ca sa combine toate categoriile.
    virtual void loadWords();

public:
    WordManager() = default;
    virtual ~WordManager() = default;

    // Interfata pe care o implementeaza fiecare categorie (metode virtuale pure).
    [[nodiscard]] virtual std::string getCategoryKey() const = 0;  // cheia din hash table
    [[nodiscard]] virtual std::string getDisplayName() const = 0;  // numele afisat jucatorului
    [[nodiscard]] virtual std::string getDataFile() const = 0;     // ex. "data/animal_words.txt"

    void selectRandomWordFromCategory();

    [[nodiscard]] const std::string& getSelectedWord() const;
    [[nodiscard]] bool isLetterInWord(char letter) const;
    [[nodiscard]] std::size_t getWordCount() const;

    // Transforma o linie bruta intr-un cuvant valid ("  River\r" -> "river").
    // Intoarce un sir gol daca linia nu contine un cuvant acceptabil.
    static std::string normalizeWord(const std::string& rawLine);
};

class AnimalWordManager : public WordManager {
public:
    [[nodiscard]] std::string getCategoryKey() const override;
    [[nodiscard]] std::string getDisplayName() const override;
    [[nodiscard]] std::string getDataFile() const override;
};

class TechnologyWordManager : public WordManager {
public:
    [[nodiscard]] std::string getCategoryKey() const override;
    [[nodiscard]] std::string getDisplayName() const override;
    [[nodiscard]] std::string getDataFile() const override;
};

class GeographyWordManager : public WordManager {
public:
    [[nodiscard]] std::string getCategoryKey() const override;
    [[nodiscard]] std::string getDisplayName() const override;
    [[nodiscard]] std::string getDataFile() const override;
};

// Categoria implicita ("Mixt"): combina toate fisierele existente, iar daca
// niciunul nu poate fi citit foloseste o lista de rezerva scrisa in cod.
class DefaultWordManager : public WordManager {
protected:
    void loadWords() override;
public:
    [[nodiscard]] std::string getCategoryKey() const override;
    [[nodiscard]] std::string getDisplayName() const override;
    [[nodiscard]] std::string getDataFile() const override;
};

// Simple Factory: singurul loc din program care stie ce clasa concreta
// corespunde fiecarei categorii. Restul codului lucreaza doar cu WordManager.
enum class Category { Animals, Technology, Geography, Mixed };

std::unique_ptr<WordManager> createWordManager(Category category);

#endif //WORD_MANAGER_HPP
