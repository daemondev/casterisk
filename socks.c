/*--------------------------------------------------
* #include <stdio.h>
* #include <sys/socket.h>
* #include <netinet/in.h>
* #include <netdb.h> //todo: not necessary
* #include <string.h>
*--------------------------------------------------*/

#ifdef WIN32
#include <Winsock2.h>
/*#include <windows.h>*/
/*#include <ws2tcpip.h>*/
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <stdio.h>
/*#include <string.h>*/
/*#include <stdint.h>*/


int main(){
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    /*socklen_t addr_size;*/
    /*addr_size = sizeof serverAddr;*/
#ifdef WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(5038);
    /* Set the IP address to desired host to connect to */
    //serverAddr.sin_addr.s_addr = inet_addr("172.16.16.10");
    /*serverAddr.sin_addr.s_addr = inet_addr("192.168.3.105");*/
    serverAddr.sin_addr.s_addr = inet_addr("190.117.113.7");
    /* Set all bits of the padding field to 0 */
    /*memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);*/

    /*---- Connect the socket to the server using the address struct ----*/

    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));

    char response[] =  "Action: Login\r\nUsername: richar\r\nSecret: @admjds.progressive\r\nActionID: 1\r\n\r\n";
    send(clientSocket, response, strlen(response), 0);

    /*close(clientSocket);*/
    /*---- Read the message from the server into the buffer ----*/

    //recv(clientSocket, buffer, 8192, 0);

    /*---- Print the received message ----*/
    int flag = 0;
    int res = 0;
    do {
        memset(buffer, 0, 1024);
        res = recv(clientSocket, buffer, 1024, 0);
        /*res = recv(clientSocket, buffer, 8192, 0);*/
        /*printf("Data received: [%s - %d]\n", buffer, res);*/

        /*char dst[res];*/
        /*memcpy(dst, buffer, strlen(buffer)+1);*/
        /*printf(dst);*/

        printf(buffer);

        if (flag == 0) {
            char ping[] = "Action: Ping\r\nActionID: 2\r\n\r\n";
            send(clientSocket, ping, strlen(ping), 0);
            flag = 1;
        }
    } while (res != 0);

    return 0;
}

/*void ascii(){*/
    /*char *str = buffer;*/
    /*while (*str) {*/
        /*printf("%c %u %x\n", *str, *str, *str);*/
        /*str++;*/
    /*}*/
/*}*/
