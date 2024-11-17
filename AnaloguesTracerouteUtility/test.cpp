#include "gtest/gtest.h"
#include "gmock/gmock.h" 
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include "./traceroute.h"

#pragma comment(lib, "ws2_32.lib")
extern int icmpSocket;


TEST(ChecksumTest, CorrectChecksum) {

    uint8_t data[] = { 8, 0, 0, 0, 0, 0, 0, 0 };
    uint16_t checksum = calculateChecksum(data, sizeof(data));
    EXPECT_NE(checksum, 0);

}
TEST(TraceRouteTest, TestTraceRoute) {

    char IP[] = "8.8.8.8";

    ASSERT_NO_THROW(TraceRoute(IP));
}
