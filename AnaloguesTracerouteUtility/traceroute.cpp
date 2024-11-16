#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include "traceroute.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int icmpSocket;
DWORD currentProcessId = GetCurrentProcessId();

void TraceRoute(char* IP) {
    cout << "Tracing..." << endl;

    struct sockaddr_in targetAddress {}; // целевой адрес
    targetAddress.sin_family = AF_INET; // IPv4

    // Преобразуем строку IP в бинарный формат
    if (inet_pton(AF_INET, IP, &targetAddress.sin_addr) <= 0) {
        cout << "Invalid IP address" << endl;
        return;
    }

    // Создание сырого сокета
    icmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmpSocket == INVALID_SOCKET) {
        cout << "Socket creation failed. Error: " << WSAGetLastError() << endl;
        return;
    }

    struct icmpHeader icmpPacket {};
    char icmpResponseBuffer[1024]; // буфер для хранения ответов ICMP
    int timeToLive = 1;

    for (; timeToLive <= 30; timeToLive++) {
        setsockopt(icmpSocket, IPPROTO_IP, IP_TTL, (const char*)&timeToLive, sizeof(timeToLive));

        icmpPacket.type = 8; // ICMP Echo Request
        icmpPacket.code = 0;
        icmpPacket.checksum = 0;
        icmpPacket.identifier = static_cast<uint16_t>(currentProcessId);
        icmpPacket.sequence = static_cast<uint16_t>(timeToLive);

        // Привести пакет к байтовому указателю
        const char* packetData = reinterpret_cast<const char*>(&icmpPacket);
        size_t packetSize = sizeof(icmpPacket);
        // Вычислить контрольную сумму
        icmpPacket.checksum = calculateChecksum(packetData, packetSize);

        // Отправить данные
        sendto(icmpSocket, packetData, packetSize, 0,
            reinterpret_cast<struct sockaddr*>(&targetAddress), sizeof(targetAddress));

        struct sockaddr_in responseAddress {}; // адрес удаленного узла
        socklen_t responseAddressLength = sizeof(responseAddress);

        // Получаем длину сообщения из полученного сокета
        int receivedLength = recvfrom(
            icmpSocket,
            icmpResponseBuffer,
            sizeof(icmpResponseBuffer),
            0,
            (struct sockaddr*)&responseAddress,
            &responseAddressLength
        );

        // Проверяем, была ли успешной операция
        if (receivedLength > 0) {
            char ipAddressString[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &responseAddress.sin_addr, ipAddressString, sizeof(ipAddressString));
            cout << timeToLive << "   " << ipAddressString << endl;

            if (responseAddress.sin_addr.s_addr == targetAddress.sin_addr.s_addr) {
                cout << "The end" << endl;
                break; // Завершаем трассировку, если достигли цели
            }
        }
        else {
            cout << timeToLive << " there is no access" << endl;
        }
    }

    closesocket(icmpSocket);
    WSACleanup(); // Завершение работы с Winsock
}

uint16_t calculateChecksum(const void* data, size_t len) {
    auto dataPtr = reinterpret_cast<const uint16_t*>(data);
    uint32_t sum = 0;

    while (len > 1) {
        sum += *dataPtr++;
        len -= 2;
    }
    if (len) {
        const uint8_t* bytePointer = reinterpret_cast<const uint8_t*>(dataPtr);
        uint8_t singleByteValue = *bytePointer;
        sum += singleByteValue;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    return static_cast<uint16_t>(~sum);
};