TCPEchoClient: main.c DieWithMessage.c TCPClientUtility.c Practical.h TCPServerUtility.c AddressUtility.c Practical.h
	gcc -o ring -std=gnu99 main.c DieWithMessage.c TCPClientUtility.c TCPServerUtility.c AddressUtility.c Practical.h

clean:
	rm -f ring
