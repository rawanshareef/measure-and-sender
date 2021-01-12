#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 5060 //The port that the server listens
#define SIZE 1024
#define size 256

void to_send(FILE *f, int socket, int count, int count2)
{
    char buf[SIZE];
    while ((fread(buf, 1, sizeof(buf), f)) > 0)
    {
        double bytesSent = send(socket, buf, sizeof(buf), 0);
        if (bytesSent == -1)
        {
            perror("[-]send error");
            exit(1);
        }
        // printf("the bytes Sent %lf.\n", bytesSent);

        // bzero(buf, SIZE);
        count++;
    }
    //sleep(2);
    if (count == 5 && count2 == 0) //CC
    {
        count2 = 1;
        long len = 0;
        strcpy(buf, "reno");
        len = strlen(buf);
        if (setsockopt(socket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0)
        {
            perror("setsockopt");
            return;
        }
    }
        printf("File sent successfully:))))). \n");

}

int main()
{
    char buf[size];
    memset(buf, 0, sizeof(buf));
    int i = 0;

    //creat a socket
    while (i < 10)
    {

        int sock;
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address)); //reset the values
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(SERVER_PORT);
        server_address.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

        sock = socket(AF_INET, SOCK_STREAM, 0); //sock stream its for TCP

        if (sock == -1)
        {
            perror("socket faild ");
            return -1;
        }

        int rval = inet_pton(AF_INET, (const char *)SERVER_IP_ADDRESS, &server_address.sin_addr);
        if (rval <= 0)
        {
            printf("inet_pton() failed");
            return -1;
        }

        //make conction
        int connect_statuse;
        connect_statuse = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
        if (connect_statuse == -1) //if cant connect its return -1
        {
            perror("[-]Error in connect");
            exit(1);
        }
        else
        {
            printf("[+]connect succes.\n");
        }

        //send
        int count = 0;
        int count2 = 0;

        FILE *myfile;
        myfile = fopen("1mb.txt", "r"); //"r" its mean just to read file
        if (myfile != NULL)
        {
            //  printf("size  of file %d \n", lengt);
            // printf("num of byte is:%d\n", bytesSent);

            to_send(myfile, sock, count, count2);
            count++;
            printf("the file successfull\n");
        }
        else
        {
            perror("can't open the file");
            exit(1);
        }
        i++;
        close(sock);
        //printf(" close the sock\n");
    }
    return 0;
}