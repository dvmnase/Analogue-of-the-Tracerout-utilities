#pragma once

#ifndef TRACEROUTE_H
#define TRACEROUTE_H
#pragma comment(lib, "ws2_32.lib")
#include <cstdint>
using namespace std;
// Структура для ICMP-заголовка
struct icmpHeader {
    uint8_t type;      // Тип сообщения ICMP
    uint8_t code;      // Код сообщения ICMP
    uint16_t checksum; // Контрольная сумма
    uint16_t identifier; // Идентификатор запроса
    uint16_t sequence;   // Номер последовательности
};

void TraceRoute(char* IP);
uint16_t calculateChecksum(const void* data, size_t len);

#endif 