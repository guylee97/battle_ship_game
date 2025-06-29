// GameServer.h
#pragma once

#include <winsock2.h>  // ������ ���� API
#pragma comment(lib, "ws2_32.lib")  // ��Ŀ ���� (WS2_32.lib ���)
#include "GameRoom.h"

extern Room* room;  // ��room�̶�� �����Ͱ� ��򰡿� �־��� �˷���


// GameServer Ŭ����: ������ ����, ���ε�, ����, Ŭ���̾�Ʈ ���� ���� ���
class GameServer {
public:
    void Start();  // ���� ���� �Լ�

private:
    SOCKET serverSocket;  // ���� ������ ����

    void InitSocket();     // ���� �ʱ�ȭ �� ���ε�
    void AcceptClients();  // Ŭ���̾�Ʈ ���� ����
};
