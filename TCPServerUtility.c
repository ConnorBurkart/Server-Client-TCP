#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

char echoedMessage[BUFSIZE]; // Global variable to store the echoed message
char globalBuffer[BUFSIZE];

int SetupTCPServerSocket(const char *service) {
  // Construct the server address structure
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

  int servSock = -1;
  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create a TCP socket
    servSock = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (servSock < 0)
      continue;       // Socket creation failed; try next address

    // Bind to the local address and set socket to listen
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      // Print local address of socket
      struct sockaddr_storage localAddr;
      socklen_t addrSize = sizeof(localAddr);
      if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
        DieWithSystemMessage("getsockname() failed");
      break;       // Bind and listen successful
    }

    close(servSock);  // Close and try again
    servSock = -1;
  }

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  return servSock;
}

int AcceptTCPConnection(int servSock) {
  struct sockaddr_storage clntAddr; // Client address
  // Set length of client address structure (in-out parameter)
  socklen_t clntAddrLen = sizeof(clntAddr);

  // Wait for a client to connect
  int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
  if (clntSock < 0)
    DieWithSystemMessage("accept() failed");

  return clntSock;
}

void HandleTCPClient(int clntSocket) 
{
  char buffer[BUFSIZE]; // Buffer for echo string
  ssize_t numBytesRcvd, numBytesSent;

  strcpy(echoedMessage, "");

  struct sockaddr_storage clntAddr;
  socklen_t clntAddrLen = sizeof(clntAddr);

  // Get client's address information
  if (getpeername(clntSocket, (struct sockaddr *)&clntAddr, &clntAddrLen) < 0) 
  {
    DieWithSystemMessage("getpeername() failed");
  }


  // Receive message from client
  numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
  {
    DieWithSystemMessage("recv() failed");
  }
  else
  {
    strncpy(echoedMessage, buffer, numBytesRcvd);
    echoedMessage[numBytesRcvd] = '\0';
  }

  // This parses the data into the first message and the rest

  echoedMessage[numBytesRcvd] = '\0'; 
  char* newline = strchr(echoedMessage, '\n');
  if (newline != NULL) 
  {
    *newline = '\0';
    strncpy(globalBuffer, newline + 1, BUFSIZE - (newline - echoedMessage) - 1);
    globalBuffer[BUFSIZE - (newline - echoedMessage) - 1] = '\0'; 
  }

      // Receive thank you note from client
    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0)
    {
        DieWithSystemMessage("recv() failed");
    }
    else
    {
        buffer[numBytesRcvd] = '\0';
        printf("Received a thank you note from client: %s\n", buffer);
    }

  /// Output
  printf("Received Data From: ");
  PrintIPAddress((struct sockaddr *)&clntAddr, stdout);
  printf("\n%s\n", echoedMessage);
  //Output



        // Send thank you note to server
    const char* thankYouNote = "Thank you";
    printf("Sending a thank you note to: ");
    PrintIPAddress((struct sockaddr *)&clntAddr, stdout);
   printf("\n");
    numBytesSent = send(clntSocket, thankYouNote, strlen(thankYouNote), 0);
    if (numBytesSent < 0) 
    {
        DieWithSystemMessage("send() failed");
    } 
    else if (numBytesSent != strlen(thankYouNote)) 
    {
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    }


  close(clntSocket); 
}
