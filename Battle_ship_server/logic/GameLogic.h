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

    // �� �÷��̾� ����: [�÷��̾�][x][y]
    std::vector<std::vector<std::vector<int>>> boards;

    // �� �÷��̾��� ���� ����
    std::vector<bool> isAlive;
};
