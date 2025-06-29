// GameServer.cpp
#include "GameServer.h"

#include <iostream>
#include <thread>
#include "ClientSession.h"


// 서버 포트 번호
#define PORT 7777

std::shared_ptr<Room> room = nullptr;


// 서버 시작
void GameServer::Start() {
    // 윈속 초기화
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup 실패: " << result << std::endl;
        return;
    }

    InitSocket();

    // GameRoom 인스턴스 생성
    room = std::make_shared<Room>();

    // 클라이언트 수락 스레드 실행
    std::thread(&GameServer::AcceptClients, this).detach();

    //
    std::thread([this]() {
        std::string command;
        while (true) {
            std::getline(std::cin, command);
            if (command == "start") {
                if (room) {
                    std::cout << "[GameServer] 강제 게임 시작 명령 수신" << std::endl;
                    room->ForceStartGame();
                }
            }
            else {
                std::cout << "[GameServer] 알 수 없는 명령입니다: " << command << std::endl;
            }
        }
        }).detach();
}

// 소켓 생성 및 포트에 바인딩
void GameServer::InitSocket() {
    // TCP 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패" << std::endl;
        WSACleanup();
        exit(1);
    }

    // 서버 주소 설정
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // 모든 IP로부터 접속 허용
    serverAddr.sin_port = htons(PORT);        // 포트번호는 네트워크 바이트 순서로 변환

    // 바인딩
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "바인딩 실패" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    // 리스닝 시작
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "리스닝 실패" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "[GameServer] 포트 " << PORT << "에서 대기 중..." << std::endl;
}

// 클라이언트 수락 루프 (무한 대기)
void GameServer::AcceptClients() {
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "클라이언트 수락 실패" << std::endl;
            continue;
        }

        std::cout << "[GameServer] 클라이언트 접속됨! 소켓: " << clientSocket << std::endl;

        // 나중에 ClientSession에서 따로 처리하게 될 예정
        auto session = std::make_shared<ClientSession>(clientSocket, room);
        room->AddClient(session);  // GameRoom에 추가
        session->Start();
    }
}
