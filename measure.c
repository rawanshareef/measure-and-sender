#include <stdio.h>
// link with Ws2_32.lib
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <time.h>

#include <sys/time.h>
#define SERVER_PORT 5060 //The port that the server listens
#define size 1024

void write_the_f(int socket)
{
  char buffer[size];

  while (recv(socket, buffer, size, 0) != 0)
  {
    bzero(buffer, size);
  }
  return;
}

int main()
{
  char buf[256];
  signal(SIGPIPE, SIG_IGN); // on linux to prevent crash on closing socket

  //step 1
  // Open the listening (server) socket
  int listeningSocket = -1;

  if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("Could not create listening socket : %d", errno);
  }

  // Reuse the address if the server socket on was closed
  // and remains for 45 seconds in TIME-WAIT state till the final removal.
  //
  int enableReuse = 1;
  if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) < 0)
  {
    printf("setsockopt() failed with error code : %d", errno);
  }
  // "sockaddr_in" is the "derived" from sockaddr structure
  // used for IPv4 communication. For IPv6, use sockaddr_in6
  //
  struct sockaddr_in serverAddress;

  memset(&serverAddress, 0, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(SERVER_PORT); //network order

  // Bind the socket to the port with any IP at this port
  if (bind(listeningSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
  {
    printf("Bind failed with error code : %d", errno);
    // TODO: close the socket
    return -1;
  }

  printf("Bind() success\n");

  //step 2
  // Make the socket listening; actually mother of all client sockets.
  if (listen(listeningSocket, 500) == -1) //500 is a Maximum size of queue connection requests
                                          //number of concurrent connections
  {
    printf("listen() failed with error code : %d", errno);
    // TODO: close the socket
    return -1;
  }
  //step 3
  //Accept and incoming connection
  printf("Waiting for incoming TCP-connections...\n");

  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen = sizeof(clientAddress);
  int count = 0;
  int total;
  double t = 0;
  double arr[10];
  int x = 0;
  int flag = 0;
  struct timeval start, stop;
  double accum;
  while (1)
  {

    while (count < 5)
    {

      memset(&clientAddress, 0, sizeof(clientAddress));
      clientAddressLen = sizeof(clientAddress);
      int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
      if (clientSocket == -1)
      {
        printf("listen failed with error code : %d", errno);
        // TODO: close the sockets
        return -1;
      }
      /* if ( count == 5)
    {

      printf("A new client connection accepted\n");

      socklen_t len;
      len = sizeof(buf);
      if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
      {
        perror("getsockopt");
        return -1;
      }

      printf("Current: %s\n", buf);
    }*/

      //Reply to client

      gettimeofday(&start, 0);
      write_the_f(clientSocket);
      count++;
      gettimeofday(&stop, 0);
      long seconds = stop.tv_sec - start.tv_sec;
      long microseconds = stop.tv_usec - start.tv_usec;
      t = seconds + microseconds * 1e-6;
      printf("the time in seconds: %lf\n", t);
      arr[x] = t;
      x++;

      if (count == 5)
      {
        double sum = 0;
        for (int i = 0; i < 5; i++)
        {
          sum = sum + arr[i];
        }
        printf("the AVG is : %lf\n", sum / 5.0);
      }
    }
    //CC

    if (count == 5)
    {

      socklen_t len;

      strcpy(buf, "reno");
      len = strlen(buf);
      if (setsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0)
      {
        perror("setsockopt");
        return -1;
      }
      len = sizeof(buf);
      if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
      {
        perror("getsockopt");
        return -1;
      }
      printf("New: %s\n", buf);
    }

    double t2 = 0;
    struct timeval start2, stop2;
    double accum2;

    while (count >= 5 && count < 11)
    {
      memset(&clientAddress, 0, sizeof(clientAddress));
      clientAddressLen = sizeof(clientAddress);
      int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
      if (clientSocket == -1)
      {
        printf("listen failed with error code : %d", errno);
        // TODO: close the sockets
        return -1;
      }
      gettimeofday(&start2, 0);
      write_the_f(clientSocket);
      count++;
      gettimeofday(&stop2, 0);
      long seconds = stop2.tv_sec - start2.tv_sec;
      long microseconds = stop2.tv_usec - start2.tv_usec;
      t2 = seconds + microseconds * 1e-6;
      printf("the time in seconds: %lf\n", t2);
      arr[x] = t2;
      x++;

      if (count == 10)
      {
        double sum = 0;
        for (int i = 4; i < 10; i++)
        {
          sum = sum + arr[i];
        }
        printf("the AVG is: %lf\n", sum / 5.0);
      }

      if (count == 10)
      {
        return;
      }
    }
    close(listeningSocket);
    printf("done :)");
    return 0;
  }
  return 0;
}