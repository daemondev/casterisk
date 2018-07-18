#ifdef WIN32
/*#include <Winsock2.h>*/
#include <winsock.h>
/*#include <windows.h>*/
/*#include <ws2tcpip.h>*/
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <sys/types.h>

#define PORT 3550
#define MAXDATASIZE 100

int main(int argc, char *argv[]) {
   int fd, numbytes;
   char buf[MAXDATASIZE];
   struct hostent *he;
   struct sockaddr_in server;

#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

   if (argc !=2) {
      /* esto es porque nuestro programa sólo necesitará un  argumento, (la IP) */
      printf("Uso: %s <Dirección IP>\n",argv[0]);
      exit(-1);
   }
   if ((he=gethostbyname(argv[1]))==NULL){
      printf("gethostbyname() error\n");
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){
      printf("socket() error\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT);
   server.sin_addr = *((struct in_addr *)he->h_addr);
   /*he->h_addr pasa la información de ``*he'' a "h_addr" */
    /*bzero(&(server.sin_zero),8);*/
    memset(server.sin_zero, '\0',8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){
      printf("connect() error\n");
      exit(-1);
   }

   if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){
      printf("Error en recv() \n");
      exit(-1);
   }

   buf[MAXDATASIZE]='\0';

   printf("Mensaje del Servidor: %s\n",buf);
    int choice;
    char command[5], filename[20], *f;
    while(1){
        printf("Enter a choice:\n1- get\n2- put\n3- pwd\n4- ls\n5- cd\n6- quit\n");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Enter filename to get: ");
                scanf("%s", filename);
                strcpy(buf, "get ");
                strcat(buf, filename);
                send(fd, buf, MAXDATASIZE, 0);
                memset(buf, '\0', MAXDATASIZE);
                recv(fd, buf, MAXDATASIZE, 0);
                printf("SERVER RESPONSE: %s\n", buf);
        }
    }



   /*close(fd);   [> cerramos fd =) <]*/
   closesocket(fd);   /* cerramos fd =) */
}
