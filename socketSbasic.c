#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	int mysock;
	char buff[1024];
	int rval;

	sock = socket(AF_INET, SOCK_STREAM,0);

	if(sock < 0) {
		perror("Socket creation failed");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);

	if(bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("Socket bind failed");
		exit(1);
	}

	listen(sock, 5);

	do {
		mysock = accept(sock, (struct sockaddr *) 0, 0);
		if(mysock == -1)
		{
			perror("Accept failed");
		}
		else
		{
			memset(buff, 0, sizeof(buff));
			if((rval = recv(mysock, buff, sizeof(buff), 0)) < 0) 
				perror("Reading stream message error");
			else if (rval == 0)
				printf("Ending connection..\n");
			else
				printf("MSG: %s\n", buff);
		printf("Message received (rval = %d)\n", rval);
		close(mysock);
		}

	}while (1);
return 0;
}