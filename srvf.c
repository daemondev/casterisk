#ifdef WIN32
#include <Winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif
/*#include <libssh/libssh.h>*/
#include <stdio.h>
#include <sys/types.h>

#define PORT 3550 /* El puerto que será abierto */
#define BACKLOG 2 /* El número de conexiones permitidas */
#define MAXDATASIZE 100

int main(){
   int fd, fd2;
   struct sockaddr_in server;
   struct sockaddr_in client;
   int sin_size;

#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
      printf("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT);
   server.sin_addr.s_addr = INADDR_ANY;

    /*bzero(&(server.sin_zero),8);*/
    memset(server.sin_zero, '0',8);

   /* A continuación la llamada a bind() */
   if(bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }

   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }

    char buf[MAXDATASIZE];
   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      if ((fd2 = accept(fd,(struct sockaddr *)&client, &sin_size))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }
        printf("Se obtuvo una conexión desde %s\n", inet_ntoa(client.sin_addr) );
        send(fd2,"Bienvenido a mi servidor.\n",22,0);

        recv(fd2, buf, MAXDATASIZE, 0);
        printf("RECEIVED FROM CLIENTE: %s\n",buf);
        closesocket(fd2);
      /*free(fd2); [> cierra fd2 <]*/
   }
}
