#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#ifdef WIN32
  #include <winsock.h>
  #include <winsock2.h>
  //#include <ws2tcpip.h> Tried this too, but didn't worked...
#else
  #include <sys/socket.h>
  #include <sys/un.h>
#endif

#define SERVER  "/tmp/serversocket"
#define MAXMSG  512

int main (void) {
  int sock;
  char message[MAXMSG];
  struct sockaddr_in name;
  size_t size;
  int nbytes;

  /* Remove the filename first, it's ok if the call fails */
  unlink (SERVER);

  /* Make the socket, then loop endlessly. */

  sock = socket(AF_INET, SOCK_STREAM, 0);   // New code.
  //sock = make_named_socket (SERVER);    // Old code.
  while (1) {
    /* Wait for a datagram. */
    size = sizeof (name);
    nbytes = recvfrom (sock, message, MAXMSG, 0,
                       (struct sockaddr *) & name, &size);
    if (nbytes < 0) {
      perror ("recfrom (server)");
      exit (EXIT_FAILURE);
    }

    /* Give a diagnostic message. */
    fprintf (stderr, "Server: got message: %s\n", message);

    /* Bounce the message back to the sender. */
    nbytes = sendto (sock, message, nbytes, 0,
                     (struct sockaddr *) & name, size);
    if (nbytes < 0) {
      perror ("sendto (server)");
      exit (EXIT_FAILURE);
    }
  }
}
