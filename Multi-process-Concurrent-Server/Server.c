#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

#define PORT 7777
#define BUF_SIZE 2048

void deal_child(int sig_no)
{
    for(;;){
        if(waitpid(-1, NULL, WNOHANG) == 0)
            break;
    }
}


int main()
{
	int listend;
	struct sockaddr_in client;
	struct sockaddr_in server;
	char send_buf[BUF_SIZE];
	char recv_buf[BUF_SIZE];
	char cmd[2048];
	int sendnum;
	int recvnum;
	int len;
	int length;
	int port;
	int opt;
	int connectd;
	pid_t pid;

	//preprocess
	memset(send_buf,0,2048);
	memset(recv_buf,0,2048);
	port = PORT;
	opt = SO_REUSEADDR;
	length = sizeof(struct sockaddr_in);

	//listen socket
	if((listend = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("create listen socket error\n");
		exit(1);
	}

	//set tcp socket option
	setsockopt(listend,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	//configure socket
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	//bind socket
	if (-1==bind(listend,(struct sockaddr *)&server,sizeof(struct sockaddr)))
	{
		perror("bind error\n");
		exit(1);
	}

	if(-1==listen(listend,5))
	{
		perror("listen error\n");
		exit(1);
	}

	while(1)
	{
		memset(recv_buf,0,2048);
		memset(send_buf,0,2048);

		if (-1==(connectd=accept(listend,(struct sockaddr*)&client,&len)))
		{
			perror("create connect socket error\n");
			continue;
		}
        signal(SIGCHLD, deal_child);
		pid = fork();

		if(0==pid)
        {
            close(listend);
            while(1)
            {
                if(0> send(connectd,send_buf,sendnum,0))
                {
                        perror("send error\n");
                        close(connectd);
                        continue;
                }

                if (0>(recvnum = recv(connectd,recv_buf,sizeof(recv_buf),0)))
                {
                        perror("recv error\n");
                        close(connectd);
                        continue;
                }

                recv_buf[recvnum] = '\0';
                strcpy(send_buf,recv_buf);

                printf("the recv message is: %s\n",recv_buf);

                send(connectd,send_buf,2048, 0);
            }
            close(connectd);
            exit(0);
        }
        else if(pid > 0)
        {
            close(connectd);
            continue;
        }

	}
}






