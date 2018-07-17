#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MINGW32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

int main() {
#ifdef MINGW32
WSADATA wsadata;
if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR) {
printf("Error creating socket.");
return -1;
}
#endif

int listen_fd;
if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
puts("Error opening socket.");
return -1;
}

struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
server_addr.sin_port = htons(80);

struct sockaddr_in client_addr;
unsigned int client_addr_length = sizeof(client_addr);

int on = 1;
setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
sizeof(on));
if ((bind(listen_fd, (struct sockaddr*)&server_addr,
sizeof(server_addr))) < 0) {
puts("Error binding socket.");
return -1;
}

if ((listen(listen_fd, 64)) < 0) {
puts("Error listening on socket.");
return -1;
}

while (1) {
int socket_fd = accept(listen_fd, (struct sockaddr *)&client_addr,
&client_addr_length);

char request[32768];
#ifdef MINGW32
recv(socket_fd, request, 32768, 0);
#else
read(socket_fd, request, 32768);
#endif

puts(request);

char* content = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nA
web server doing POST<br/><form method=post><input type=text
name=something value=something><input type=submit value=Submit></
form>";
#ifdef MINGW32
send(socket_fd, content, strlen(content), 0);
closesocket(socket_fd);
#else
write(socket_fd, content, strlen(content));
close(socket_fd);
#endif

}

#ifdef MINGW32
closesocket(listen_fd);
WSACleanup();
#endif
return 0;
}
