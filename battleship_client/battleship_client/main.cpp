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
            std::cerr << "[�ý���] ���� ���� �����\n";
            break;
        }
        buffer[recvLen] = '\0';
        std::cout << "\n[���� ����] " << buffer << "\n> ";
        std::cout.flush();
    }
}

int main() {
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup ����\n";
        return 1;
    }

    // ���� ����
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "���� ���� ����\n";
        WSACleanup();
        return 1;
    }

    // ���� �ּ� ����
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);  // ��Ʈ ��ȣ
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "IP �ּ� ��ȯ ����\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ������ ����
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "���� ���� ����\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[�ý���] ������ ����Ǿ����ϴ�.\n";
    std::cout << "��ǥ�� �Է��ϼ��� (��: 2 3)\n";

    // ���� ������ ����
    std::thread recvWorker(RecvThread, sock);
    recvWorker.detach();  // ��׶��� ����

    // �Է� ����
    while (true) {
        int x, y;
        std::cout << "> ";
        if (!(std::cin >> x >> y)) {
            std::cerr << "�Է� ���� �Ǵ� ����\n";
            break;
        }

        std::string msg = std::to_string(x) + " " + std::to_string(y);
        send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
