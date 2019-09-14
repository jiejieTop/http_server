#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define PORT 7777
#define BUF_SIZE 2048

void print_usage(char* str)
{
	printf("%s usage :\n",str);
	printf("%s Ipaddr \n");

}

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in client;
	struct sockaddr_in server;
	char send_buf[BUF_SIZE];
	char recv_buf[BUF_SIZE];
	int sendnum;
	int recvnum;
	int length;
	int port;
	int len;
	int opt;
	int ret;
	
	if(2!=argc)
	{
		print_usage(argv[0]);
		exit(1);	

	}
	
    memset(send_buf,0,2048);
	memset(recv_buf,0,2048);
	port = PORT;
 	length = sizeof(struct sockaddr);	

	if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("can not create socket\n");
		exit(1);
	}
		
        memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	if (0>(ret=connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))))
	{
		perror("connect error");
		close(sockfd);
		exit(1);

	}
	
    while(1)
	{
		printf("udp>");
		gets(send_buf);
		printf("\n");
		
        if (0>(len=send(sockfd,send_buf,strlen(send_buf),0)))
        {
            perror("send data error\n");
            close(sockfd);
            exit(1);

        }

		if (0==strcmp(send_buf,"quit"))
		{
			perror("quitting remote ctroling\n");
			break;
		}
		
        if (0>(len=recv(sockfd,recv_buf,2048,0)))
        {
            perror("recv data error\n");
            close(sockfd);
            exit(1);
        }
		
		fprintf(stdout,"%s\n",recv_buf);

	}
	close(sockfd);
	exit(1);
}

