#ifdef WIN32
#include <Winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

//int sendFile(int, char[]);

int main(int argc, char *argv[]) {
    argv[0]="2";
    argv[1]="190.117.113.7"; //my ip

    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    char message[100]="USER francisco\nPASS jeiko333\n";
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    argc=2;

    if(argc != 2) {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(21);

    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    if( send(sockfd , message , strlen(message) , 0) < 0) {
        perror("send failed");
        return 1;
    }

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF) {
            printf("\n Error : Fputs error\n");
            return 0;
        }
    }

    if(n < 0) {
        printf("\n Read error \n");
        return 0;
    }

    return 0;
}

/*
int sendFile(int socket, char text[]) {
    FILE *fp;
    int size =0, k, status, nsize = 0, temp_size, i, sent, numbytes, lala;
    char buf[20], file_buf[505], temp_buf[505];
    char ch;
    struct stat info;

	nsize = strlen(text);
	char name[nsize];
	lala = strlen(name);
	strncpy(name, text, nsize-1);
	name[nsize-1] = '\0';
	lala = strlen(name);

	if((fp = fopen(name, "r")) == NULL) {
	    perror("Can't open file");
	    text[0] = '/';
	    write(socket,text, 80);
	    return 1;
	}
	write(socket,text, 80);
	printf("Sending File: %s\n",text);

	status = stat( name,&info );

	if( status == -1 ){
	    printf(" fstat failed \n");
	    return 1;
	}
	size = info.st_size;
	temp_size = size;

	for( i=0; i<=9; i++ )   {
	    buf[i] =  size%10;
	    size = size/10;
	}

	if (send(socket, buf, 10, 0) == -1){
	    perror("Cant send file size...\n\n");
	    return 1;
	}


	i = 0;
	sent = 0;
	while( sent <= temp_size ){
	    ch = fgetc(fp);
	    if( sent == temp_size ){
		if (i>0){
		    if (( numbytes = send(socket, file_buf, i, 0)) == -1)
			perror("Error in sending file.....\n\n\n");
		}
		    break;
	    }

	    file_buf[i++] = ch;

	    if(i == MAX_BUFSIZE){
		if (( numbytes = send(socket, file_buf, MAX_BUFSIZE, 0)) == -1)
			perror("Error in sending file....\n\n\n");
		    i = 0;
	    }
		sent++;
	}

	fclose(fp);
}

*/


/*
int receiveFile(int socket, char text[]) {
FILE *fp;
int nsize,k, i, size=0, temp_size, mul = 1, numbytes, lala, recvd;
int count_freq, repeat;
char ch,buf[20],file_buf[505], temp_buf[505];
saferead(socket, text,80);
if(text[0] == '/')
    return 1;


nsize = strlen(text);
char name[nsize];
strncpy(name, text, nsize-1);
name[nsize-1] = '\0';

if ((fp = fopen(name,"w")) == NULL) {
        perror(" Can't open file");
    return 1;
}

if ((numbytes=recv(socket, buf, 10, 0)) == -1) {
    perror("Cant receive size of the file....\n\n");
    return 1;
}

for( i=0; i<=9; i++ ){
    size =  size + ( buf[i]*mul );
    mul = mul * 10;
}


recvd = 0;
printf("Received File: %s\n", name);
repeat = size/MAXDATASIZE;

if ( repeat>50 )
    count_freq = repeat/50;

i=0;
while( recvd < size ){

    if ((numbytes=recv(socket, temp_buf, MAXDATASIZE, 0)) == -1) {
        perror("Error receiving file from server....\n\n");
        return 1;
    }

    fwrite( temp_buf, sizeof(temp_buf[0]), numbytes, fp );
    recvd = recvd + numbytes;
}
fclose(fp);
}
/**/
