// GameServer.cpp
#include "GameServer.h"

#include <iostream>
#include <thread>
#include "ClientSession.h"


// ���� ��Ʈ ��ȣ
#define PORT 7777

std::shared_ptr<Room> room = nullptr;


// ���� ����
void GameServer::Start() {
    // ���� �ʱ�ȭ
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup ����: " << result << std::endl;
        return;
    }

    InitSocket();

    // GameRoom �ν��Ͻ� ����
    room = std::make_shared<Room>();

    // Ŭ���̾�Ʈ ���� ������ ����
    std::thread(&GameServer::AcceptClients, this).detach();

    //
    std::thread([this]() {
        std::string command;
        while (true) {
            std::getline(std::cin, command);
            if (command == "start") {
                if (room) {
                    std::cout << "[GameServer] ���� ���� ���� ��� ����" << std::endl;
                    room->ForceStartGame();
                }
            }
            else {
                std::cout << "[GameServer] �� �� ���� ����Դϴ�: " << command << std::endl;
            }
        }
        }).detach();
}

// ���� ���� �� ��Ʈ�� ���ε�
void GameServer::InitSocket() {
    // TCP ���� ����
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "���� ���� ����" << std::endl;
        WSACleanup();
        exit(1);
    }

    // ���� �ּ� ����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // ��� IP�κ��� ���� ���
    serverAddr.sin_port = htons(PORT);        // ��Ʈ��ȣ�� ��Ʈ��ũ ����Ʈ ������ ��ȯ

    // ���ε�
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "���ε� ����" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    // ������ ����
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "������ ����" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "[GameServer] ��Ʈ " << PORT << "���� ��� ��..." << std::endl;
}

// Ŭ���̾�Ʈ ���� ���� (���� ���)
void GameServer::AcceptClients() {
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Ŭ���̾�Ʈ ���� ����" << std::endl;
            continue;
        }

        std::cout << "[GameServer] Ŭ���̾�Ʈ ���ӵ�! ����: " << clientSocket << std::endl;

        // ���߿� ClientSession���� ���� ó���ϰ� �� ����
        auto session = std::make_shared<ClientSession>(clientSocket, room);
        room->AddClient(session);  // GameRoom�� �߰�
        session->Start();
    }
}
