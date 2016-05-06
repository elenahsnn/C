#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define DATA "Hello World of Socket"
#define LOW_DELAY 0x10
#define HIGH_THROUGHPUT 0x08
#define HIGH_RELIABILITY 0x04
#define BLOCKING 0
#define UNBLOCKING 1

int main(int argc, char *argv[])
{
	int sock, ret, val, len, mode;
	struct sockaddr_in server;
	struct hostent *hp;
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

	int cnt, addrLen, on=1;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	memset(&server, 0, sizeof(server));
	server.sin_port = htons(BCAST_PORT);
	server.sin_addr.s_addr = inet_addr("255.255.255.255");
	addrLen = sizeof(server);
	cnt = sendto(sock, buffer, strlen(buffer), 0,(struct sockaddr_in *)&server, addrLen);

	setsockopt( sock, SOL_SOCKET, SO_DEBUG, &on, sizeof(on));

	len =sizeof(val);
	ret = getsockopt(sock, SOL_SOCKET, SO_DONTROUTE,(void *)&val, &len);
	printf("so_dontroute = %d\n", val);
	val = 1;
	ret = setsockopt(sock, SOL_SOCKET, SO_DONTROUTE,(void *)&val, sizeof(int));

	ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, (void *),&val,&len);
	printf("so_error = %d\n", val);

	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 10;
	ret = setsockopt(sock, SOL_SOCKET, SO_LINGER,(void *)&ling, sizeof(ling));
	ret = close(sock);

	ret = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on));

	int value = 32768;
	ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&value, sizeof(value));
	ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&value, sizeof(value));

	int value1 = 48;
	ret = setsockopt(sock, SOL_SOCKET, SO_RCVLOWAT, (void *)&value1, sizeof(value1));

	ret = setsockopt(sock, SOL_SOCKET, SO_SNDLOWAT, (void *),&value1, sizeof(value1));

	struct timeval timeo;
	len = sizeof(timeo);
	ret = getsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeo, &len);
	printf("Timeout %d seconds, %d microseconds \n", timeo.tv_sec, timeo.tv_usec);

	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(MY_PORT);
	ret = bind(sock,(structsockaddr *)&server, sizeof(server));
	printf("Bind returned %d\n", ret);

	int interval =1;
	ret = setsockopt(sock, IPPROTO_TCP, TCP_KEEPALIVE,(void *)&interval, sizeof(interval));

	int duration = 1;
	ret = setsockopt(sock, IPPROTO_TCP, TCP_KEEPALIVE,(void *)&duration, sizeof(duration));

	int off = 1;
	ret = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&off, sizeof(off));

	int sz =128;
	ret = setsockopt(sock, IPPROTO_TCP, TCP_MAXSEG, (void *)&sz, sizeof(sz));

	int tos;
	tos = (HIGH_THROUGHPUT | HIGH_RELIABILITY);
	ret = setsockopt(sock, IPPROTO_IP, IP_TOS,(void *)&tos, sizeof(tos));

	int ttl = 1;
	ret = setsockopt(sock, IPPROTO_IP, IP_TTL,(void *)&ttl, sizeof(ttl));

	struct ip_mreq mreq;
	bzero((void *)&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.253");
	mreq.imr_interface.s_addr = htonl (INADDR_ANY);
	ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void *)&mreq, sizeof(mreq));

	struct in_addr intf_addr;
	intf_addr.s_addr = inet_addr("192.168.1.2");
	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,(void *)&intf_addr, sizeof(intf_addr));

	int mttl = 10;
	ret = stsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&mttl, sizeof(mttl));

	int on1 = 0;
	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (void *)&on, sizeof(on));

	char oobdata;
	ret = setsockopt(sock, SOL_SOCKET, SO_OOBINLINE,&on, sizeof(on));
	if(sockatmark(sock)){
		ret = read(sock, &oobdata, 1);
	}
	else{
		ret = read(sock, buffer, sizeof(buffer));
	}

	mode = NONBLOCKING;
	ret = ioctl(sock, FIONBIO, &mode);

	char buffer[MAX_BUFFER_SIZE+1];
	ret = recv(sock, buffer, MAX_BUFFER_SIZE, 0);
	if(ret >0)
		printf("Data received");
	else if(ret <0)
		printf("Error occurred. Please check error status");
	else if(ret ==0){
		printf("Peer closed.");
		close(sock);
	}

	int servsock, clisock, size;
	servsock = socket(AF_INET, SOCK_STREAM, 0);
	clisock = accept(servsock, (struct sockaddr *)NULL, NULL);
	ret = write(sock, buffer, size, 0);
	if(ret == size)
		printf("Data written");
	else if(ret <0){
		perror("error");
		if(errno ==EPIPE){
			printf("Peer closed");
			close(clisock);
		}
	}

	char address[] = {"192.168.1.1"};
	server.sin_addr.s_addr = inet_addr(address);
	if(server.sin_addr.s_addr = 0xffffffff){
		struct hostent *hptr = (struct hostent *)gethistbyname(address);
		if(hptr == NULL){
			printf("CANT RESOLVE ADDRESS");
		}
		else{
			struct in_addr **addrs;
			addrs =(struct in_addr **)hptr->h_addr_list;
			memcpy(&saddr.sin_addr, *addrs, sizeof(struct in_addr));
		}
	}
return 0;
}

