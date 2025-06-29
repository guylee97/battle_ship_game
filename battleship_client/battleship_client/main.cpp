// ClientMain.cpp
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void RecvThread(SOCKET sock) {
    char buffer[512];
    while (true) {
        int recvLen = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (recvLen <= 0) {
            std::cerr << "[시스템] 서버 연결 종료됨\n";
            break;
        }
        buffer[recvLen] = '\0';
        std::cout << "\n[서버 응답] " << buffer << "\n> ";
        std::cout.flush();
    }
}

int main() {
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup 실패\n";
        return 1;
    }

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패\n";
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);  // 포트 번호
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "IP 주소 변환 실패\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 서버에 연결
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "서버 연결 실패\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[시스템] 서버에 연결되었습니다.\n";
    std::cout << "좌표를 입력하세요 (예: 2 3)\n";

    // 수신 스레드 시작
    std::thread recvWorker(RecvThread, sock);
    recvWorker.detach();  // 백그라운드 실행

    // 입력 루프
    while (true) {
        int x, y;
        std::cout << "> ";
        if (!(std::cin >> x >> y)) {
            std::cerr << "입력 오류 또는 종료\n";
            break;
        }

        std::string msg = std::to_string(x) + " " + std::to_string(y);
        send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
