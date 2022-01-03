/************************************************************/
/* This is a stream socket server sample program for UNIX   */
/* domain sockets. This program listens for a connection    */
/* from a client program, accepts it, reads data from the   */
/* client and prints it.   */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH  "/tmp/fabian"

int main(void){
    int server_sock, client_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    //Incoming Connection Requests will be saved in a queue of Length 10
    int backlog = 10;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        perror("SOCKET ERROR: ");
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = SUN_LEN(&server_sockaddr);

    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        perror("BIND ERROR: ");
        close(server_sock);
        exit(1);
    }

    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(server_sock, backlog);
    if (rc == -1){
        perror("LISTEN ERROR: ");
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");

    /*********************************/
    /* Accept an incoming connection */
    /*********************************/
    len = SUN_LEN(&server_sockaddr);
    socklen_t clientLen = SUN_LEN(&client_sockaddr);

    while (1) {
      client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &clientLen);
      if (client_sock == -1){
          perror("ACCEPT ERROR: ");
          close(server_sock);
          close(client_sock);
          exit(1);
      }

      /************************************/
      /* Read and print the data          */
      /* incoming on the connected socket */
      /************************************/
      printf("waiting to read...\n");
      //also possible with read() instead of recv()
      bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
      if (bytes_rec == -1){
          perror("RECV ERROR: ");
          close(server_sock);
          close(client_sock);
          exit(1);
      }
      else {
          printf("DATA RECEIVED = %s\n", buf);
      }

      close(client_sock);
    }


    /******************************/
    /* Close the sockets and exit */
    /******************************/
    close(server_sock);

    return 0;
}
