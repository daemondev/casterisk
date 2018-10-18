#include <stdio.h>
#include "winsock2.h"
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

void main() {
    int size;
    char buffer[1024];

    WSADATA wsaData;
    sockaddr_in service;
    SOCKET ListenSocket, AcceptSocket;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(10011);

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));
    listen(ListenSocket, SOMAXCONN);

    AcceptSocket = accept(ListenSocket, NULL, NULL);
    cout << "Client connected.\n";

    size = recv(AcceptSocket, buffer, 1024, 0);
    string msg(buffer, size);
    /*
    msg contains this:
        GET / HTTP/1.1
        Host: 172.16.199.150:10011
        Connection: Upgrade
        Pragma: no-cache
        Cache-Control: no-cache
        Upgrade: websocket
        Origin: http://localhost
        Sec-WebSocket-Version: 13
        User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36
        Accept-Encoding: gzip, deflate, sdch
        Accept-Language: en-US,en;q=0.8,hu;q=0.6
        Sec-WebSocket-Key: 34qTtPYjnRJheHKQowePRg==
        Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
    */

    string abc = "abc";
    auto cmsg = abc.c_str();
    size = send(AcceptSocket, cmsg, abc.size(), 0);
    // size == 3, which means send was successful, but client doesn't receive anything

    // hangs here, even tho client sent a message
    size = recv(AcceptSocket, buffer, 1024, 0);
}
