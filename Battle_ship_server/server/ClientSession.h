// ClientSession.h
#pragma once

#include <winsock2.h>
#include <string>

class Room;  // 전방 선언


class ClientSession {
public:
    ClientSession(SOCKET socket, std::shared_ptr<Room> room);
    ~ClientSession();

    void Start();
    void Send(const std::string& message);
    SOCKET GetSocket() const;  // 
    bool IsReady() const { return isReady; }
    void MarkReady() { isReady = true; }

private:
    bool isReady = false;

private:
    void ReceiveLoop();

    SOCKET clientSocket;
    bool isActive;
    std::shared_ptr<Room> room;
};

