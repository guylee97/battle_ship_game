// ClientSession.cpp
#include "ClientSession.h"
#include <iostream>
#include <thread>

ClientSession::ClientSession(SOCKET socket, Room* r)
    : clientSocket(socket), room(r), isActive(true) {}


ClientSession::~ClientSession() {
    closesocket(clientSocket);  // ���� ����
    std::cout << "[ClientSession] ���� �����" << std::endl;
}

void ClientSession::Start() {
    std::thread(&ClientSession::ReceiveLoop, this).detach();  // ���� ���� ���� ������� ����
}

void ClientSession::ReceiveLoop() {
    MarkReady();  // recv ���۰� ���ÿ� Ready �÷��� ����

    char buffer[1024];

    while (isActive) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cout << "[ClientSession] ���� ���� �Ǵ� ���� ����" << std::endl;
            isActive = false;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string receivedMessage(buffer);

        std::cout << "[ClientSession] ���� �޽���: " << receivedMessage << std::endl;

        // Echo ���� ������
        Send("�����κ��� ����: " + receivedMessage);
    }
}

void ClientSession::Send(const std::string& message) {
    if (clientSocket == INVALID_SOCKET) return;

    try {
        int result = send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
        if (result == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cerr << "[����] send ����: " << err << std::endl;
            isActive = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[����] Send �� ���� �߻�: " << e.what() << std::endl;
        isActive = false;
    }
}

SOCKET ClientSession::GetSocket() const {
    return clientSocket;
}
