// Room.cpp
#include "GameRoom.h"
#include "ClientSession.h"
#include "GameLogic.h"
#include <algorithm>
#include <iostream>

// 생성자: GameLogic 초기화
Room::Room() : gameStarted(false) {}

// 클라이언트를 방에 추가하는 함수
// - 최대 3명까지만 수용 가능
// - 성공하면 true, 가득 찼으면 false
bool Room::AddClient(std::shared_ptr<ClientSession> client) {
    std::lock_guard<std::mutex> lock(roomMutex);
    if (clients.size() >= MAX_CLIENTS)
        return false;

    clients.push_back(client);

    // 방이 가득 차면 게임 시작
    if (clients.size() == MAX_CLIENTS && !gameStarted) {
        StartGame();
    }
    return true;
}

// 방에 있는 모든 클라이언트에게 메시지를 브로드캐스트
void Room::Broadcast(const std::string& message) {
    std::cout << "[디버그] Broadcast 호출됨 - this: " << this << std::endl;
    std::lock_guard<std::mutex> lock(roomMutex);
    for (auto it = clients.begin(); it != clients.end(); ) {
        if (!(*it)->IsReady()) {
            it = clients.erase(it);
        }
        else {
            (*it)->Send(message);
            ++it;
        }
    }
}

// 클라이언트가 연결 종료되었을 때 리스트에서 제거
void Room::RemoveClient(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(roomMutex);
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [clientSocket](std::shared_ptr<ClientSession>& client) {
                return client->GetSocket() == clientSocket;
            }),
        clients.end());
}

// 방이 가득 찼는지 확인
bool Room::IsFull() {
    std::lock_guard<std::mutex> lock(roomMutex);
    return clients.size() >= MAX_CLIENTS;
}

// 게임 시작 처리 함수
// - GameLogic 초기화 및 플레이어 알림
void Room::StartGame() {
    gameStarted = true;

    // 모든 클라이언트가 Ready 될 때까지 최대 3초 대기
    const int maxWaitMillis = 3000;
    const int stepMillis = 100;

    int waited = 0;
    while (waited < maxWaitMillis) {
        bool allReady = true;
        for (auto& client : clients) {
            if (!client->IsReady()) {
                allReady = false;
                break;
            }
        }

        if (allReady) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(stepMillis));
        waited += stepMillis;
    }

    gameLogic.InitializeGame(clients.size());

    for (size_t i = 0; i < clients.size(); ++i) {
        clients[i]->Send("[안내] 게임 시작! 당신은 Player " + std::to_string(i + 1));
    }

    Broadcast("[안내] 모든 플레이어가 입장했습니다. 게임을 시작합니다!");
}

// 클라이언트의 공격을 처리하는 함수
// - 예: 좌표 2,3을 공격 → 게임 로직 결과 → 모든 클라이언트에게 전달

void Room::HandleAttack(SOCKET attackerSocket, int x, int y) {
    std::lock_guard<std::mutex> lock(roomMutex);

    int attackerIndex = -1;

    // 공격자의 인덱스를 찾음
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->GetSocket() == attackerSocket) {
            attackerIndex = static_cast<int>(i);
            break;
        }
    }

    if (attackerIndex == -1) return; // 공격자 못 찾음

    // GameLogic을 통해 공격 처리
    std::string resultMessage = gameLogic.Attack(attackerIndex, x, y);

    // 결과를 전체에 알림
    Broadcast("[공격결과] " + resultMessage);

    // 게임 종료 여부 확인
    if (gameLogic.IsGameOver()) {
        Broadcast("[안내] 게임이 종료되었습니다!");
        gameStarted = false;
    }
    else {
        // 다음 턴으로 진행
        gameLogic.NextTurn();
        Broadcast("[안내] 다음 턴: 플레이어 " + std::to_string(gameLogic.GetCurrentTurn()));
    }
}

void Room::ForceStartGame() {
    std::lock_guard<std::mutex> lock(roomMutex);
    if (!gameStarted && !clients.empty()) {
        StartGame();
    }
    else {
        std::cout << "[Room] 게임이 이미 시작되었거나 플레이어 없음" << std::endl;
    }
}
