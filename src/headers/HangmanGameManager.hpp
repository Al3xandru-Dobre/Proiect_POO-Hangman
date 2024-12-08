//
// Created by Alex on 08.12.2024.
//

#ifndef HANGMANGAMEMANAGER_HPP
#define HANGMANGAMEMANAGER_HPP
#include "Game.hpp"
#include "Hint_Game.hpp"
#include "Word_Manager.hpp"
#include "Game_Timer.hpp"
#include "Leaderboard.hpp"
#include "Hangman_UI.hpp"
#include <memory>


class HangmanGameManager {
    std::unique_ptr <Scor> jucator;
    std::unique_ptr<WordManager> wordManager;
    std::unique_ptr <HintGame> hintGame;
    Game game;
    std::unique_ptr<HangmanUI> ui;
    Leaderboard leaderboard;
    void selecteazaCategorie();
    Game gameInstance();
public:

    HangmanGameManager();
    void startJoc();
    Game* operator->();
    const Game* operator->() const;
    virtual ~HangmanGameManager() = default;
};
#endif //HANGMANGAMEMANAGER_HPP
