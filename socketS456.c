#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAX(x,y) ((x>y) ? x : y)

int main(int argc, char *argv[])
{
	int sock, s1, s2, s3, ret, max, on;
	struct sockaddr_in server;
	struct sockaddr_in sa1, sa2;
	int mysock;
	char buff[1024];
	int rval;
	fd_set rfds;

	sock = socket(AF_INET, SOCK_STREAM,0);
	s1 = socket(AF_INET, SOCK_STREAM,0);
	s2 = socket(AF_INET,SOCK_STREAM,0);

	max = MAX(s1,s2) +1;
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

	on =1;
	ret = setsockopt(s1, SOL_SOCKET, SO_REUSEADDR,&on,sizeof(on));
	ret = setsockopt(s2, SOL_SOCKET, SO_REUSEADDR,&on,sizeof(on));

	memset(&sa1, 0, sizeof(sa1));
	memset(&sa2, 0, sizeof(sa2));

	sa1.sin_family = sa2.sin_family = AF_INET;
	sa1.sin_port = sa2.sin_port = sa2.sin_port = htons(8192);
	sa1.sin_addr.s_addr = inet_addr("192.168.1.1");
	sa2.sin_addr.s_addr = inet_addr("10.0.0.1");

	ret = bind(s1,(struct sockaddr *)&sa1, sizeof(sa1));
	ret = listen(s1,5);

	ret = bind(s2,(struct sockaddr *)&sa2, sizeof(sa2));
	ret = listen(s2,5);

	while(1){
		FD_ZERO(&rfds);
		FD_SET(s1, &rfds);
		FD_SET(s2, &rfds);

		ret= select(max, &rfds, NULL, NULL, NULL);

		if(ret > 0){
			if(FD_ISSET(s1, &rfds)){
				printf("Received connect request over 192.168.1.1\n");
				s3 = accept(s1,(struct sockaddr_in *)NULL, NULL);
				close(s3);
			}
			else if(FD_ISSET(s2, &rfds){
				printf("Received connect request over 10.0.0.1\n");
				s3 = accept(s2,(struct sockaddr_in *)NULL,NULL);
				close(s3);
			}
			else
				printf("error!\n");
		}
	}
	close(s1);
	close(s2);
return 0;
}
