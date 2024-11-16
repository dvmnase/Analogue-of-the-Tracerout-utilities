#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include "traceroute.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        cout << "WSAStartup failed with error: " << wsaResult << endl;
        return 1; // Завершаем программу с ошибкой
    }
    char* IP = argv[1];
   // char* IP = nullptr;
    TraceRoute(IP);
    WSACleanup(); //Завершение работыс Winock
    return 0;
}

