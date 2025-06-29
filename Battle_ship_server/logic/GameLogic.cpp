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
    // 각 플레이어의 보드에 무작위로 배 한 칸 배치
    for (int p = 0; p < playerCount; ++p) {
        int x = std::rand() % boardSize;
        int y = std::rand() % boardSize;
        boards[p][x][y] = 1;  // 1은 배
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
    } while (!isAlive[currentTurn]);  // 생존자만 턴을 넘김
}

// 새로운 플레이어 수에 따라 게임 초기화 (보드 리셋 및 상태 초기화)
void GameLogic::InitializeGame(int newPlayerCount) {
    playerCount = newPlayerCount;
    currentTurn = 0;
    boards = std::vector<std::vector<std::vector<int>>>(playerCount, std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0)));
    isAlive = std::vector<bool>(playerCount, true);

    StartGame();  // 게임 시작 시 배도 배치
}

// 공격 로직: 공격자는 현재 턴의 플레이어, 대상은 자동 결정
std::string GameLogic::Attack(int attackerIndex, int x, int y) {
    int targetIndex = -1;

    // 공격 대상을 찾음 (자기 자신 제외한 생존자 중 첫 번째)
    for (int i = 0; i < playerCount; ++i) {
        if (i != attackerIndex && isAlive[i]) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) return "공격할 대상이 없습니다.";

    if (boards[targetIndex][x][y] == 1) {
        boards[targetIndex][x][y] = -1;
        isAlive[targetIndex] = false;
        return "명중! 상대방이 탈락했습니다.";
    }
    else {
        return "빗나갔습니다.";
    }
}

// 게임 종료 여부 확인
bool GameLogic::IsGameOver() const {
    int aliveCount = 0;
    for (bool alive : isAlive) {
        if (alive) ++aliveCount;
    }
    return aliveCount <= 1;
}