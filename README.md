Connor Burkart
CPSC 3600
Assigment 1
2/27/24


# HOW TO RUN

1. Make
2. Client:
```
./ring <IP> <Port> <FileName>
```
3. Server:
```
 /ring <IP> <Port>
```
4. Make clean


For the Client you need a file. It does not matter what it is as long as it holds data. You will need to create it dyanmically.
# Basic Ideas 

How I did the file handling is import everything into a string. Sent that gaint file to server.
The server takes the first line and stores it in the message. Then the rest of the message gets
stored in a global string. The first message is deleted then the rest remains so when the next
call it takes the now first part (was the 2nd message) and sets everything up for the 3rd terminal.

Used sleep to switch between the modes.

Used PrintOwnIPAddress for the first line of output and if the terminal was talking about itself.
Then used the parameter when finding what IP to send to and recieved the thank you note.
Then when inside of HandleTCPClient we use PrintIPAddress to say who we recived it from and how we sent the thank you note.


# ADDED FUNCTIONS 

## main.c

### void BecomeServer(char *service,char *server)
	This simply takes the code from TCPServerEcho.c and makes it a function.

### void BecomeClient(const char *server, const char *service,const char *filename)
	This is the client function that is from the starterfiles.

### void PrintOwnIPAddress()
	This is one of the functions I made to get the IP of the machine. It is extremely simple and round about way
	to get this but is noninvasive.

### char* readFileContents(const char* filename)
	This function takes the file and loads in all into a string. Then reuturns the string. This is all the file
	handling we do.

## TCPServerUtility.c

### void removeSpaces(char* source)
	This will remove the white spaces in the char to just get the message.

## AddressUtility.c

### void PrintIPAddress(const struct sockaddr *address, FILE *stream)
	This is built off of PrintSocketAddress(). We however just need the IP and not the socket too.
	I was sacred to edit the first function so I basically just copied it and made it just print the IP
	We only use this when are printing others IPs.


