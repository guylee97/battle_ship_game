// ClientSession.cpp
#include "ClientSession.h"
#include <iostream>
#include <thread>

ClientSession::ClientSession(SOCKET socket, Room* r)
    : clientSocket(socket), room(r), isActive(true) {}


ClientSession::~ClientSession() {
    closesocket(clientSocket);  // 연결 종료
    std::cout << "[ClientSession] 연결 종료됨" << std::endl;
}

void ClientSession::Start() {
    std::thread(&ClientSession::ReceiveLoop, this).detach();  // 수신 루프 별도 스레드로 실행
}

void ClientSession::ReceiveLoop() {
    MarkReady();  // recv 시작과 동시에 Ready 플래그 설정

    char buffer[1024];

    while (isActive) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cout << "[ClientSession] 수신 실패 또는 연결 종료" << std::endl;
            isActive = false;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string receivedMessage(buffer);

        std::cout << "[ClientSession] 받은 메시지: " << receivedMessage << std::endl;

        // Echo 응답 보내기
        Send("서버로부터 응답: " + receivedMessage);
    }
}

void ClientSession::Send(const std::string& message) {
    if (clientSocket == INVALID_SOCKET) return;

    try {
        int result = send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
        if (result == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cerr << "[오류] send 실패: " << err << std::endl;
            isActive = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[예외] Send 중 예외 발생: " << e.what() << std::endl;
        isActive = false;
    }
}

SOCKET ClientSession::GetSocket() const {
    return clientSocket;
}
