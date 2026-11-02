cmake_minimum_required(VERSION 3.24)
project(hangman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HANGMAN_BUILD_GUI   "Construieste interfata grafica (SFML 3)" ON)
option(HANGMAN_BUILD_TESTS "Construieste testele pentru logica jocului" ON)

# ---------------------------------------------------------------------------
# 1. Nucleul jocului (logica pura, fara nicio dependenta grafica)
# ---------------------------------------------------------------------------
add_library(hangman_core STATIC
        src/cpp/Word_Manager.cpp
        src/cpp/Animal.cpp
        src/cpp/Technology.cpp
        src/cpp/Geografie.cpp
        src/cpp/Default_Word_Manager.cpp
        src/cpp/Hint_Game.cpp
        src/cpp/Game_Timer.cpp
        src/cpp/Game.cpp
        src/cpp/scor.cpp
        src/cpp/Leaderboard.cpp
        src/cpp/Resources.cpp
)
target_include_directories(hangman_core PUBLIC src)
# Folderul sursa este folosit ca rezerva pentru gasirea fisierelor data/ si assets/,
# astfel incat jocul porneste corect si din cmake-build-debug (directorul CLion).
target_compile_definitions(hangman_core PUBLIC HANGMAN_RESOURCE_DIR="${CMAKE_SOURCE_DIR}")

if (MSVC)
    target_compile_options(hangman_core PUBLIC /W4 /utf-8)
else ()
    target_compile_options(hangman_core PUBLIC -Wall -Wextra -Wpedantic)
endif ()

# ---------------------------------------------------------------------------
# 2. Executabilul (consola + GUI optional)
# ---------------------------------------------------------------------------
set(HANGMAN_SOURCES main.cpp src/cpp/Console_UI.cpp)

if (HANGMAN_BUILD_GUI)
    # Intai cautam un SFML 2.6 instalat (ex. apt install libsfml-dev, vcpkg, MSYS2).
    # Daca nu exista, CMake il descarca si il compileaza automat (static).
    find_package(SFML 2.6 COMPONENTS graphics window system QUIET)
    if (NOT SFML_FOUND)
        message(STATUS "SFML nu a fost gasit local -> descarc SFML 2.6.2 prin FetchContent")
        include(FetchContent)
        set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
        set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)   # link static: fara DLL-uri de copiat pe Windows
        FetchContent_Declare(SFML
                GIT_REPOSITORY https://github.com/SFML/SFML.git
                GIT_TAG 2.6.2
                GIT_SHALLOW ON)
        FetchContent_MakeAvailable(SFML)
    endif ()

    list(APPEND HANGMAN_SOURCES
            src/cpp/Hangman_UI.cpp
            src/cpp/ui/RoundedRectangleShape.cpp
            src/cpp/ui/Widgets.cpp
            src/cpp/ui/WordWidgets.cpp
            src/cpp/ui/HangmanFigure.cpp
            src/cpp/ui/ParticleSystem.cpp
            src/cpp/ui/MenuScene.cpp
            src/cpp/ui/GameScene.cpp
            src/cpp/ui/LeaderboardScene.cpp
    )
endif ()

add_executable(hangman ${HANGMAN_SOURCES})
target_link_libraries(hangman PRIVATE hangman_core)

if (HANGMAN_BUILD_GUI)
    target_link_libraries(hangman PRIVATE sfml-graphics sfml-window sfml-system)
    target_compile_definitions(hangman PRIVATE HANGMAN_WITH_GUI)
endif ()

# Copiem resursele langa executabil (pentru distribuire / rulare din alt folder).
add_custom_command(TARGET hangman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:hangman>/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:hangman>/assets)

# ---------------------------------------------------------------------------
# 3. Teste
# ---------------------------------------------------------------------------
if (HANGMAN_BUILD_TESTS)
    enable_testing()
    add_executable(hangman_tests tests/core_tests.cpp)
    target_link_libraries(hangman_tests PRIVATE hangman_core)
    add_test(NAME hangman_core_tests COMMAND hangman_tests)
endif ()
