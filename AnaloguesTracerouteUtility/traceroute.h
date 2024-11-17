#pragma once

#ifndef TRACEROUTE_H
#define TRACEROUTE_H
#pragma comment(lib, "ws2_32.lib")
#include <cstdint>
using namespace std;
// ��������� ��� ICMP-���������
struct icmpHeader {
    uint8_t type;      // ��� ��������� ICMP
    uint8_t code;      // ��� ��������� ICMP
    uint16_t checksum; // ����������� �����
    uint16_t identifier; // ������������� �������
    uint16_t sequence;   // ����� ������������������
};

void TraceRoute(char* IP);
uint16_t calculateChecksum(const void* data, size_t len);

#endif 