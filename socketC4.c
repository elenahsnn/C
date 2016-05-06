#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define DATA "Hello World of Socket"
#define MAX_HOSTNAME 255

int main(int argc, char *argv[])
{
	int sock, ret;
	struct sockaddr_in server;
	struct hostent *hp;
	struct servent *sp;
	char buff[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Socket failed");
		exit(1);
	}
	
	server.sin_family = AF_INET;

	hp = gethostbyname (argv[1]);
	if(hp == 0)
	{
		perror("gethostbyname failed");
		close(sock);
		exit(1);
	}

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(5000);

	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		perror("Connect failed");
		close(sock);
		exit(1);
	}

	if(send(sock, DATA, sizeof(DATA), 0) < 0)
	{
		perror("Send failed");
		close(sock);
		exit(1);
	 }
	printf("Sent %s\n", DATA);
	close(sock);

	fd_set readfds;
	struct timeval tv;
	char buf1[256], buf2[256];
	int select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	FD_ZERO (&readfds);
	FD_SET (sock, &readfds);
	to.tv_sec = 10;
	to.tv_usec = 0;
	ret = select (sock+1, &readfds, NULL, NULL, &to);
	if(ret == -1){
		perror("select");}
	else if (ret == 0){
		printf("Timeout occurred! No data after 10.5 secs.\n");}
	else {
		if(FD_ISSET(sock, &readfds)){
			recv(sock, buf1, sizeof buf1, 0);},
		else {
			perror("error");}


	int la_len;
	la_len = sizeof (server);
	ret = getsockname(sock,(struct sockaddr_in *)&server, &la_len);
	if(ret == 0){
		printf("Local Address is : %s\n", inet_ntoa(server.sin_addr));
		printf("Local Port is : %d\n", server.sin_port);
	}

	ret = getpeername(sock,(struct sockaddr_in *)&server, &la_len);
	if(ret == 0){
		printf("Peer Address is: %s\n", inet_ntoa (server.sin_addr));
		printf("Peer Port is: %d\n", server.sin_port);
	}

	int len, size;
	len = sizeof(size);
	ret = getsockopt(sock, SOL_SOCKET, SO_SNDBUF,(void *)&size, (socklen_t *)&len);
	size = size * 2;
	ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF,(void *)&size, sizeof(size));

	char hostbuffer[MAX_HOSTNAME+1]
	ret = gethostname(hostbuffer, MAX_HOSTNAME);
	if(ret == 0){
		printf("Host name is %s\n",hostbuffer);
	}

	strcpy(hostbuffer, "Elena");
	ret = sethostname(hostbuffer, strlen(hostbuffer));
	if(ret == 0){
		printf("Host name is now %s\n", hostbuffer);
	}

	inet_aton("192.168.1.1", &in);
	if( hp = gethostbyaddr((char *)&in.s_addr, sizeof(in.s_addr), AF_INET)){
		printf("Host name is %s\n", hp->h_name);
	}

	sp = getservbyname("smtp", "tcp");
	if(sp){
		printf("s_name = %s\n", sp->s_name);
		printf("s_port = %d\n", ntohs(sp->s_port));
		printf("s_proto = %s\n", sp ->s_proto);
	}

	int i = 0;
	printf("Aliases:\n");
	while(1){
		if(sp->s_aliases[i]){
			printf("s_aliases[%d] = %s\n", i, sp->s_aliases[i]);
			i++;
		}
		else 
			break;
	}

	sp = getservbyport(htons(80), NULL);
	if(sp){
		printf("Service %s is at port %d\n", sp->s_name, ntohs(sp->s_port));
	}

return 0;
}
