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


    //게임 시작 (MAX_CLIENTS가 모두 찼을 때 자동 호출 예정)
    void StartGame();

    // 공격 처리: 소켓 기반으로 공격자 인식, 타겟은 자동 선택
    void HandleAttack(SOCKET attackerSocket, int x, int y);

    void ForceStartGame();  // 강제 시작 함수 추가


private:
    std::vector<std::shared_ptr<ClientSession>> clients;
    std::mutex roomMutex;
    static const int MAX_CLIENTS = 3;

    GameLogic gameLogic;             
    bool gameStarted = false;        
};