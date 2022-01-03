/************************************************************/
/* This is a stream socket client sample program for UNIX   */
/* domain sockets. This program creates a socket, connects  */
/* to a server, sends data, then receives and prints a      */
/* message from the server.                                 */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/fabian"
#define CLIENT_PATH "/tmp/fabian.client"

int main(int argc, char *argv[]){

    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1){
        perror("SOCKET ERROR: ");
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = SUN_LEN(&client_sockaddr);

    //make the port usable again, in case it already exists.. so I can send messages quickly
    unlink(CLIENT_PATH);

    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("BIND ERROR: ");
        close(client_sock);
        exit(1);
    }


    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1){
        perror("CONNECT ERROR: ");
        close(client_sock);
        exit(1);
    }

    /************************************/
    /* Copy the data to the buffer (=stringToSend) and  */
    /* send it to the server socket.    */
    /************************************/

    char stringToSend[80];
    if(strlen(argv[1]) > 79) {
      strncpy(stringToSend, argv[1], 79);
      stringToSend[79] = '\0';
    } else if(strlen(argv[1]) < 79) {
      strcpy(stringToSend, argv[1]);
      size_t prevlen = strlen(stringToSend);
      memset(stringToSend + prevlen, '\0', 79 - prevlen);
    } else {
      strcpy(stringToSend, argv[1]);
    }

    printf("Sending data...\n");
    //write() would work as well
    rc = send(client_sock, stringToSend, strlen(stringToSend), 0);
    if (rc == -1) {
        perror("SENDERROR: ");
        close(client_sock);
        exit(1);
    }
    else {
        printf("Data sent!\n");
    }

    /******************************/
    /* Close the socket and exit. */
    /******************************/
    close(client_sock);

    return 0;
}
