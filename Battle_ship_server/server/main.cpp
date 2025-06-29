// main.cpp
#include <iostream>
#include <thread>
#include "GameServer.h"  // 우리가 구현할 GameServer 클래스

int main() {
    std::cout << "배틀쉽 서버를 시작합니다..." << std::endl;

    // GameServer 클래스 인스턴스 생성
    GameServer server;

    // 서버 시작: 소켓 열고, 클라이언트 수락 대기
    server.Start();

    // 프로그램이 끝나지 않도록 무한 대기
    // 서버는 내부적으로 클라이언트를 수락하고 게임방을 운영함
    while (true) {
        // 여기에 아무 코드도 없지만,
        // 실제로는 GameServer 내부에서 스레드로 동작 중일 것
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
