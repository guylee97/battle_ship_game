// logic/GameLogic.cpp
#include "GameLogic.h"
#include <cstdlib>
#include <ctime>

GameLogic::GameLogic(int playerCount, int boardSize)
    : playerCount(playerCount), boardSize(boardSize), currentTurn(0),
    boards(playerCount, std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0))),
    isAlive(playerCount, true)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void GameLogic::StartGame() {
    // �� �÷��̾��� ���忡 �������� �� �� ĭ ��ġ
    for (int p = 0; p < playerCount; ++p) {
        int x = std::rand() % boardSize;
        int y = std::rand() % boardSize;
        boards[p][x][y] = 1;  // 1�� ��
    }
}



bool GameLogic::CheckVictory(int& winnerIndex) {
    int aliveCount = 0;
    for (int i = 0; i < playerCount; ++i) {
        if (isAlive[i]) {
            aliveCount++;
            winnerIndex = i;
        }
    }
    return aliveCount == 1;
}

int GameLogic::GetCurrentTurn() const {
    return currentTurn;
}

void GameLogic::NextTurn() {
    do {
        currentTurn = (currentTurn + 1) % playerCount;
    } while (!isAlive[currentTurn]);  // �����ڸ� ���� �ѱ�
}

// ���ο� �÷��̾� ���� ���� ���� �ʱ�ȭ (���� ���� �� ���� �ʱ�ȭ)
void GameLogic::InitializeGame(int newPlayerCount) {
    playerCount = newPlayerCount;
    currentTurn = 0;
    boards = std::vector<std::vector<std::vector<int>>>(playerCount, std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0)));
    isAlive = std::vector<bool>(playerCount, true);

    StartGame();  // ���� ���� �� �赵 ��ġ
}

// ���� ����: �����ڴ� ���� ���� �÷��̾�, ����� �ڵ� ����
std::string GameLogic::Attack(int attackerIndex, int x, int y) {
    int targetIndex = -1;

    // ���� ����� ã�� (�ڱ� �ڽ� ������ ������ �� ù ��°)
    for (int i = 0; i < playerCount; ++i) {
        if (i != attackerIndex && isAlive[i]) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) return "������ ����� �����ϴ�.";

    if (boards[targetIndex][x][y] == 1) {
        boards[targetIndex][x][y] = -1;
        isAlive[targetIndex] = false;
        return "����! ������ Ż���߽��ϴ�.";
    }
    else {
        return "���������ϴ�.";
    }
}

// ���� ���� ���� Ȯ��
bool GameLogic::IsGameOver() const {
    int aliveCount = 0;
    for (bool alive : isAlive) {
        if (alive) ++aliveCount;
    }
    return aliveCount <= 1;
}