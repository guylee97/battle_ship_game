// logic/GameLogic.h
#pragma once

#include <vector>
#include <string>

class GameLogic {
public:
    GameLogic(int playerCount = 3, int boardSize = 5);

    void StartGame();
    bool CheckVictory(int& winnerIndex);
    int GetCurrentTurn() const;
    void NextTurn();


    void InitializeGame(int newPlayerCount);


    std::string Attack(int attackerIndex, int x, int y);

 
    bool IsGameOver() const;

private:
    int playerCount;
    int boardSize;
    int currentTurn;

    // 각 플레이어 보드: [플레이어][x][y]
    std::vector<std::vector<std::vector<int>>> boards;

    // 각 플레이어의 생존 여부
    std::vector<bool> isAlive;
};
