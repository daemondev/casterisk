#ifdef WIN32
#include <Winsock2.h>
/*#include <windows.h>*/
/*#include <ws2tcpip.h>*/
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <stdio.h>

int main(){
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5038);

    serverAddr.sin_addr.s_addr = inet_addr("190.117.113.7");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));

    char response[] =  "Action: Login\r\nUsername: richar\r\nSecret: @admjds.progressive\r\nActionID: 1\r\n\r\n";
    send(clientSocket, response, strlen(response), 0);

    int flag = 0;
    int res = 0;
    do {
        memset(buffer, 0, 1024);
        res = recv(clientSocket, buffer, 1024, 0);
        printf(buffer);
        if (flag == 0) {
            char ping[] = "Action: Ping\r\nActionID: 2\r\n\r\n";
            send(clientSocket, ping, strlen(ping), 0);
            flag = 1;
        }
    } while (res != 0);

    return 0;
}
