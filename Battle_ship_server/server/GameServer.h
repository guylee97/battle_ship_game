// GameServer.h
#pragma once

#include <winsock2.h>  // 윈도우 소켓 API
#pragma comment(lib, "ws2_32.lib")  // 링커 설정 (WS2_32.lib 사용)
#include "GameRoom.h"

extern Room* room;  // “room이라는 포인터가 어딘가에 있어”라고만 알려줌


// GameServer 클래스: 서버의 생성, 바인딩, 리슨, 클라이언트 수락 등을 담당
class GameServer {
public:
    void Start();  // 서버 시작 함수

private:
    SOCKET serverSocket;  // 서버 리스닝 소켓

    void InitSocket();     // 소켓 초기화 및 바인딩
    void AcceptClients();  // 클라이언트 수락 루프
};
