#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 9502658 
//#define MAX_SIZE 65535 

int main()
{
	int sock_desc;
	struct sockaddr_in serv_addr;
	char* sbuff = (char*) malloc (MAX_SIZE);
	struct iovec   iov[1];
	struct msghdr  msg;

	FILE *fp = fopen("7M.test", "r");

	memset(&msg, 0, sizeof(msg));
	memset(iov, 0, sizeof(iov));

	memset(sbuff, 0, MAX_SIZE);

	if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Failed creating socket\n");

	bzero((char *) &serv_addr, sizeof (serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(3000);

	if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
		printf("Failed to connect to server\n");
		return -1;
	}

	iov[0].iov_base = sbuff;
	iov[0].iov_len  = MAX_SIZE;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	while(1)
	{
		fseek(fp, 0, SEEK_SET);  
		while(fgets(sbuff, MAX_SIZE , fp))
		{
			sendmsg(sock_desc, &msg, 0);
		}
	}
	free(sbuff);
	close(sock_desc);
	return 0;

}
