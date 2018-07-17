#include <stdio.h>
#include <winsock.h>

#define PORT 5038

int main(int argc, char *argv[]){
	int client_socket;
	char *host;
	struct hostent *hent;
	int iplen = 15; //XXX.XXX.XXX.XXX
	struct sockaddr_in *remote;
	char buf[1024];

	printf("Socket client example\n");

	// create tcp socket
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

	client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0) {
		printf("client_socket = %d\n", client_socket);
		perror("Can't create TCP socket\n");
		exit(1);
	}

        host = "ryr.progr.am";
	printf("Host: %s\n",host);
	char *ip = (char *)malloc(iplen+1);
	memset(ip, 0, iplen+1);
	if((hent = gethostbyname(host)) == NULL) {
		perror("Can't get IP");
		exit(1);
	}

	ip = inet_ntoa(*(struct in_addr *)*hent->h_addr_list);
	printf("The IP: %s\n",ip);

	// setup remote socket
	remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	remote->sin_family = AF_INET;
	printf("s_addr:%d\n",remote->sin_addr.s_addr);
	remote->sin_addr.s_addr = inet_addr(ip);
	remote->sin_port = htons(PORT);
	printf("s_addr:%d\n",remote->sin_addr.s_addr);

	// have to read
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737625(v=vs.85).aspx

	// connect socket
           /*connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);*/
	if(connect(client_socket, (struct sockaddr *)remote, sizeof(struct sockaddr)) == SOCKET_ERROR){
		closesocket(client_socket);
		perror("Could not connect");
		WSACleanup();
		exit(1);
	}

        char login_cmd[] =  "Action: Login\r\nUsername: richar\r\nSecret: @admjds.progressive\r\nActionID: 1\r\n\r\n";
        send(client_socket, login_cmd, strlen(login_cmd), 0);

        int flag = 0;
        int res = 0;

        do {
            memset(buf, 0, 1024);
            res = recv(client_socket, buf, 1024, 0);
            printf(buf);
            if (flag == 0) {
                char ping[] = "Action: Ping\r\nActionID: 2\r\n\r\n";
                send(client_socket, ping, strlen(ping), 0);
                flag = 1;
            }
        } while (res != 0);

	free(remote);
	closesocket(client_socket);
	WSACleanup();

	printf("Program end");

	return 0;
}
