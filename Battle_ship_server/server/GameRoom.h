// Room.h
#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include "ClientSession.h"
#include "../logic/GameLogic.h" 

class Room {
public:
    Room();

    bool AddClient(std::shared_ptr<ClientSession> client);
    void Broadcast(const std::string& message);
    void RemoveClient(SOCKET clientSocket);

    bool IsFull();


    //���� ���� (MAX_CLIENTS�� ��� á�� �� �ڵ� ȣ�� ����)
    void StartGame();

    // ���� ó��: ���� ������� ������ �ν�, Ÿ���� �ڵ� ����
    void HandleAttack(SOCKET attackerSocket, int x, int y);

    void ForceStartGame();  // ���� ���� �Լ� �߰�


private:
    std::vector<std::shared_ptr<ClientSession>> clients;
    std::mutex roomMutex;
    static const int MAX_CLIENTS = 3;

    GameLogic gameLogic;             
    bool gameStarted = false;        
};