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
    try {
       
        int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaResult != 0) {
            cerr << "WSAStartup failed with error: " << wsaResult << endl;
            return 1; 
        }

        if (argc < 2) {
            cerr << "Usage " << argv[0] << " IP ADDRESS" << endl;
            WSACleanup;
            return 1;
        }
        char* IP = argv[1];
 
        TraceRoute(IP);
        WSACleanup(); //Завершение работыс Winock
        return 0;
    }
    catch (const exception& e) {
        cerr << "Error " << e.what() << endl;
        WSACleanup();
        return 1;
    }
    catch (...) {
        cerr << "An unknown error occurred" << endl;
        WSACleanup();
        return 1;
    }
}

// получать данные из аргументов командной строки
//результат выводится на консоль
//обработка исключительных ситуаций
//модульные тесты для основных функций утилиты