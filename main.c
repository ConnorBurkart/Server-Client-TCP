#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"
#include <netinet/in.h>
#include <arpa/inet.h>


extern char echoedMessage[BUFSIZE]; // Global variable to store the echoed message
extern char globalBuffer[BUFSIZE]; // Holds what is left in the file 


int main(int argc, char *argv[]) {


    if(argc == 4) 
    {

        char *server = argv[1]; // Server address/name
        char *service = argv[2]; // Server port/service
        char *filename = argv[3]; 

        PrintOwnIPAddress();

        BecomeClient(server,service,filename);

        sleep(2);

        BecomeServer(service,server);
        printf("Goodbye!\n");


    }
    else
    {
        char *server = argv[1]; // Server address/name
        char *service = argv[2]; // Server port/service
        PrintOwnIPAddress();

        BecomeServer(service,server);

        sleep(3);


        BecomeClient(server,service,NULL);

        printf("Goodbye!\n");

    }   



}


void BecomeServer(char *service,char *server) 
{
    int servSock = SetupTCPServerSocket(service);
    if (servSock < 0) 
    {
        DieWithUserMessage("SetupTCPServerSocket() failed", service);
    }


    int clntSock = AcceptTCPConnection(servSock);
    if (clntSock >= 0) 
    {

        HandleTCPClient(clntSock);  

        close(clntSock);
    } 
    else 
    {
        DieWithSystemMessage("AcceptTCPConnection() failed");
    }



    close(servSock);
}

void BecomeClient(const char *server, const char *service,const char *filename) 
{
    const char *echoString;

    int sock = SetupTCPClientSocket(server, service);
    if (sock < 0) 
    {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
        return;
    }

    if(filename != NULL)
    {
        char* fileContents = readFileContents(filename);
        if (fileContents) 
        {

            // Sending data to server
            printf("Sending data to %s\n", server);
            ssize_t numBytes = send(sock, fileContents, strlen(fileContents), 0);
            if (numBytes < 0) 
            {
                DieWithSystemMessage("send() failed");
            } 
            else if (numBytes != strlen(fileContents)) 
            {
                DieWithUserMessage("send()", "sent unexpected number of bytes");
            }

            free(fileContents);
        }

    }
    else
    {

        char* fileContents = globalBuffer;
        // Sending data to server
        printf("Sending data to %s\n", server);
        ssize_t numBytes = send(sock, fileContents, strlen(fileContents), 0);
        if (numBytes < 0) 
        {
            DieWithSystemMessage("send() failed");
        } 
        else if (numBytes != strlen(fileContents)) 
        {
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        }

    }



    close(sock);
}


void PrintOwnIPAddress() {
    char buffer[128];
    FILE *fp = popen("hostname -I", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
        printf("My IP address is: %s", buffer);
    }

    pclose(fp);
}

char* readFileContents(const char* filename) 
{
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';

    fclose(file);
    return buffer;
}








