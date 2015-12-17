// socket server example, handles multiple clients using threads

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <signal.h> //for signal

#define MAX_SIZE 9502658 
//#define MAX_SIZE 65535 

//the thread function
void *connection_handler(void *);
typedef void (*signal_t)(int);

//signal handler
void sig_handler(int signo)
{
    printf("SIGNAL thid %d : %d\n", pthread_self(),signo);
	sleep(1);
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//puts("Socket created");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 3000 );
	
	// set reuse address options
	int one = 1;
	if (-1==setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,
				(char *)&one, sizeof(one))) {
		printf("Could not set SO_REUSEADDR");
	} 


	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	//puts("bind done");

	//Listen
	listen(socket_desc , 3);

	//Accept and incoming connection
	//puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	c=sizeof(struct sockaddr_in);
	while(client_sock=accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c))
	{
		//puts("Connection accepted");

		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		// SIGUSR1 mask
		sigset_t newmask;
		sigemptyset(&newmask);
		sigaddset(&newmask, SIGUSR1);
		pthread_sigmask(SIG_BLOCK, &newmask, NULL);

		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}

		//puts("Handler assigned");
	}

	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	return 0;
}
/*
   This will handle connection for each client
   */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int n;
	int pass_sd;

	char* sendBuff = (char*) malloc (MAX_SIZE);
	char* tmpBuff = (char*) malloc (MAX_SIZE);
	struct iovec   iov[1];
	struct msghdr  msg;
	struct sigaction act;
	sigset_t newmask;
	signal_t handler;

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	//signal(SIGUSR1, sig_handler);
	act.sa_handler = sig_handler;
	sigaction(SIGUSR1, &act, NULL);
	pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);

	memset(&msg, 0, sizeof(msg));
	memset(iov, 0, sizeof(iov));
	memset(sendBuff, 0, MAX_SIZE);
	memset(tmpBuff, 0, MAX_SIZE);

	iov[0].iov_base = sendBuff;
	iov[0].iov_len  = MAX_SIZE;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	while((n = recvmsg(sock,&msg,MSG_WAITALL)) > 0)
	{
		memcpy( tmpBuff, sendBuff, MAX_SIZE);
	}

	free(tmpBuff);
	free(sendBuff);
	close(sock);

	if(n)
	{
		perror("recvmsg failed");
	}
	return 0;
}
