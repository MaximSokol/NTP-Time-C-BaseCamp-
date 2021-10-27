#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;


void ntpDate() {

    int s, i;
    int portno = 123;
    const int maxLen = 1024;
    char* hostName = (char*)"200.20.186.76";

    unsigned char msg[48] = { 010,0,0,0,0,0,0,0,0 };
    unsigned int buf[maxLen];
    struct protoent* proto;
    struct sockaddr_in server_addr;

    proto = getprotobyname("udp");
    s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(hostName);
    server_addr.sin_port = htons(portno);

    i = sendto(s, (const char*)msg, sizeof(msg), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    struct sockaddr saddr;
    int fromlen = sizeof(saddr);
    i = recvfrom(s, (char*)buf, 48, 0, &saddr, &fromlen);

    unsigned int tmit;
    tmit = ntohl((time_t)buf[4]);
    tmit -= 2208988800U;
    cout << "NTP time is " << time(tmit) << std::endl;
    i = time(0);
    cout << "System time is " << (i - tmit) << " Seconds off" << std::endl;
}

int main() {

    WSADATA wsaData;
    if (!WSAStartup(MAKEWORD(2, 2), &wsaData))
        ntpDate();
    return 0;
}
