// Room.cpp
#include "GameRoom.h"
#include "ClientSession.h"
#include "GameLogic.h"
#include <algorithm>
#include <iostream>

// ������: GameLogic �ʱ�ȭ
Room::Room() : gameStarted(false) {}

// Ŭ���̾�Ʈ�� �濡 �߰��ϴ� �Լ�
// - �ִ� 3������� ���� ����
// - �����ϸ� true, ���� á���� false
bool Room::AddClient(std::shared_ptr<ClientSession> client) {
    std::lock_guard<std::mutex> lock(roomMutex);
    if (clients.size() >= MAX_CLIENTS)
        return false;

    clients.push_back(client);

    // ���� ���� ���� ���� ����
    if (clients.size() == MAX_CLIENTS && !gameStarted) {
        StartGame();
    }
    return true;
}

// �濡 �ִ� ��� Ŭ���̾�Ʈ���� �޽����� ��ε�ĳ��Ʈ
void Room::Broadcast(const std::string& message) {
    std::cout << "[�����] Broadcast ȣ��� - this: " << this << std::endl;
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

// Ŭ���̾�Ʈ�� ���� ����Ǿ��� �� ����Ʈ���� ����
void Room::RemoveClient(SOCKET clientSocket) {
    std::lock_guard<std::mutex> lock(roomMutex);
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [clientSocket](std::shared_ptr<ClientSession>& client) {
                return client->GetSocket() == clientSocket;
            }),
        clients.end());
}

// ���� ���� á���� Ȯ��
bool Room::IsFull() {
    std::lock_guard<std::mutex> lock(roomMutex);
    return clients.size() >= MAX_CLIENTS;
}

// ���� ���� ó�� �Լ�
// - GameLogic �ʱ�ȭ �� �÷��̾� �˸�
void Room::StartGame() {
    gameStarted = true;

    // ��� Ŭ���̾�Ʈ�� Ready �� ������ �ִ� 3�� ���
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
        clients[i]->Send("[�ȳ�] ���� ����! ����� Player " + std::to_string(i + 1));
    }

    Broadcast("[�ȳ�] ��� �÷��̾ �����߽��ϴ�. ������ �����մϴ�!");
}

// Ŭ���̾�Ʈ�� ������ ó���ϴ� �Լ�
// - ��: ��ǥ 2,3�� ���� �� ���� ���� ��� �� ��� Ŭ���̾�Ʈ���� ����

void Room::HandleAttack(SOCKET attackerSocket, int x, int y) {
    std::lock_guard<std::mutex> lock(roomMutex);

    int attackerIndex = -1;

    // �������� �ε����� ã��
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->GetSocket() == attackerSocket) {
            attackerIndex = static_cast<int>(i);
            break;
        }
    }

    if (attackerIndex == -1) return; // ������ �� ã��

    // GameLogic�� ���� ���� ó��
    std::string resultMessage = gameLogic.Attack(attackerIndex, x, y);

    // ����� ��ü�� �˸�
    Broadcast("[���ݰ��] " + resultMessage);

    // ���� ���� ���� Ȯ��
    if (gameLogic.IsGameOver()) {
        Broadcast("[�ȳ�] ������ ����Ǿ����ϴ�!");
        gameStarted = false;
    }
    else {
        // ���� ������ ����
        gameLogic.NextTurn();
        Broadcast("[�ȳ�] ���� ��: �÷��̾� " + std::to_string(gameLogic.GetCurrentTurn()));
    }
}

void Room::ForceStartGame() {
    std::lock_guard<std::mutex> lock(roomMutex);
    if (!gameStarted && !clients.empty()) {
        StartGame();
    }
    else {
        std::cout << "[Room] ������ �̹� ���۵Ǿ��ų� �÷��̾� ����" << std::endl;
    }
}
