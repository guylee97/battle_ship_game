// main.cpp
#include <iostream>
#include <thread>
#include "GameServer.h"  // �츮�� ������ GameServer Ŭ����

int main() {
    std::cout << "��Ʋ�� ������ �����մϴ�..." << std::endl;

    // GameServer Ŭ���� �ν��Ͻ� ����
    GameServer server;

    // ���� ����: ���� ����, Ŭ���̾�Ʈ ���� ���
    server.Start();

    // ���α׷��� ������ �ʵ��� ���� ���
    // ������ ���������� Ŭ���̾�Ʈ�� �����ϰ� ���ӹ��� ���
    while (true) {
        // ���⿡ �ƹ� �ڵ嵵 ������,
        // �����δ� GameServer ���ο��� ������� ���� ���� ��
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
