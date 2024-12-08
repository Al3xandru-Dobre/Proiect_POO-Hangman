cmake_minimum_required(VERSION 3.29)
project(hangman_demo_v0_1)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/src/headers)
add_executable(hangman_demo_v0_1 main.cpp
        src/cpp/scor.cpp
        src/headers/scor.hpp
        src/cpp/Leaderboard.cpp
        src/headers/Leaderboard.hpp
        src/cpp/Game_Timer.cpp
        src/headers/Game_Timer.hpp
        src/cpp/Word_Manager.cpp
        src/headers/Word_Manager.hpp
        src/cpp/Game.cpp
        src/headers/Game.hpp
        src/cpp/Hint_Game.cpp
        src/headers/Hint_Game.hpp
        src/cpp/Hangman_UI.cpp
        src/headers/Hangman_UI.hpp
        src/cpp/Geografie.cpp
        src/headers/Technology.hpp
        src/cpp/Technology.cpp
        src/cpp/Animal.cpp
        src/cpp/DefaultWordManager.cpp
        src/headers/HangmanGameManager.hpp
        src/cpp/HangmanGameManager.cpp
        src/cpp/invalidInputError.cpp
        src/headers/invalidInputException.hpp
)
