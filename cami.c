#ifdef WIN32
#include <Winsock2.h>
/*#include <windows.h>*/
/*#include <ws2tcpip.h>*/
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>

/*char *match(char *, char *);*/

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
        /*printf(buffer);*/

        /*if(strstr(buffer, "PeerStatus: Reachable") != NULL){*/
        if(strstr(buffer, "PeerStatus: Registered") != NULL){
            char *fp = strchr(buffer, '/');
            const char sep[2] = "\n";
            char *token;

            token = strtok(fp, sep);

            printf("#######> REGISTERED :%s:\n", fp);
            while(token != NULL){
                printf("token: %s\n", token);
                /*memset(token, 0, 200);*/
                token = strtok(NULL, sep);
            }

            /*printf("#######> REGISTERED %s\n",strrchr(buffer, "Peer: SIP/"));*/

        }

        if (flag == 0) {
            if(strstr(buffer, "Message: Authentication accepted") != NULL){
                char ping[] = "Action: Ping\r\nActionID: 2\r\n\r\n";
                send(clientSocket, ping, strlen(ping), 0);
                flag = 1;
            }
        }
    } while (res != 0);

    return 0;
}
/*--------------------------------------------------
* char *match(char *string, char *pattern){
*     regex_t regex;
*     int reti;
*     char msgbuf[100];
*
*     reti = regcomp(&regex, "^a[[:alnum:]]", 0);
*     / *reti = regcomp(&regex, pattern, 0);* /
*     if (reti) {
*         fprintf(stderr, "Could not compile regex\n");
*         exit(1);
*     }
*
*     reti = regexec(&regex, string, 0, NULL, 0);
*     if (!reti) {
*         puts("Match");
*     } else if (reti == REG_NOMATCH) {
*         puts("No match");
*     } else {
*         regerror(reti, &regex, msgbuf, sizeof(msgbuf));
*         fprintf(stderr, "Regex match failed: %s\n", msgbuf);
*         exit(1);
*     }
*
*     regfree(&regex);
* }
*--------------------------------------------------*/

/*#include <regex.h>*/
/*gcc cami.c -lws2_32 -I"K:\local\bin\mingw64\opt\include" -L"K:\local\bin\mingw64\opt\lib" -lregex -o cami.exe*/
/*gcc cstate.c -lws2_32 -I"C:\Program Files\PostgreSQL\9.4\include" -L"C:\Program Files\PostgreSQL\9.4\lib" -lpq -o cstate*/
