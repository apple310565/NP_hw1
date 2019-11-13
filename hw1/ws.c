#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SERV_PORT 80 //http port
#define BUFFERSIZE 8096

struct
{
	char *ext;
	char *filetype;
}
extensions [] =
{
	{"gif", "image/gif" },
	{"jpg", "image/jpeg"},
	{"jpeg","image/jpeg"},
	{"png", "image/png" },
	{"zip", "image/zip" },
	{"gz",  "image/gz"  },
	{"tar", "image/tar" },
	{"htm", "text/html" },
	{"html","text/html" },
	{"exe","text/plain" },
	{0,0}
};

void handle_socket(int fd)
{
	int j, file_fd, buflen, len;
	long i, ret;
	char * fstr;
	static char buffer[BUFFERSIZE+1];

	ret=read(fd,buffer,BUFFERSIZE); //讀取瀏覽器需求
	if(ret==0 || ret==-1)//連線有問題，結束
	{
		perror("read");
		exit(3);
	}

	//處理字串:結尾補0,刪換行
	if(ret>0 && ret<BUFFERSIZE)
		buffer[ret]=0;
	else
		buffer[0]=0;
	for(i=0;i<ret;i++)
	{
		if(buffer[i]=='\r' || buffer[i]=='\n')
			buffer[i] = 0;
	}

	//判斷GET命令
	if(strncmp(buffer,"GET ",4)&&strncmp(buffer,"get ",4))
		exit(3);

	//將HTTP/1.0隔開
	for(i=4;i<BUFFERSIZE;i++)
	{
		if(buffer[i]==' ')
		{
			buffer[i]=0;
			break;
		}
	}

	//要求根目錄,讀取html
	if (!strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) )
		strcpy(buffer,"GET /index.html\0");

	//檢查客戶端所要求的檔案格式
	buflen = strlen(buffer);
	fstr = (char *)0;

	for(i=0;extensions[i].ext!=0;i++)
	{
		len=strlen(extensions[i].ext);
		if(!strncmp(&buffer[buflen-len], extensions[i].ext, len))
		{
			fstr=extensions[i].filetype;
			break;
		}
	}
	//檔案格式不支援
	if(fstr==0)
		fstr=extensions[i-1].filetype;

	//開檔
	if((file_fd=open(&buffer[5],O_RDONLY))==-1)
		write(fd, "Failed to open file", 19);

	//回傳 200OK 內容格式
	sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
	write(fd,buffer,strlen(buffer));

	//讀檔，輸出到客戶端
	while((ret=read(file_fd, buffer, BUFFERSIZE))>0)write(fd,buffer,ret);

	exit(1);
}
int main(void)
{

	pid_t pid;
	int sockfd,sockfd2;
	struct sockaddr_in addr_server,addr_client;
	socklen_t length;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket error");
		exit(1);
	}
	bzero(&addr_server,sizeof(addr_server));
	addr_server.sin_family=AF_INET;
	addr_server.sin_addr.s_addr=htonl(INADDR_ANY);
	addr_server.sin_port = htons(SERV_PORT);
	if(bind(sockfd,(struct sockaddr *)&addr_server,sizeof(addr_server))<0)
	{
		perror("bind error");
		exit(2);
	}

	listen(sockfd, 5);//最大連線數

	printf("Accept connect...\n");

	while(1)
	{
		length = sizeof(addr_client);
		//等待客戶端連線
		if((sockfd2 = accept(sockfd, (struct sockaddr *)&addr_client, &length))<0)
		{
			perror("accept error");
			exit(3);
		}


		//fork() children
		if((pid = fork()) < 0)exit(3);
		else
		{
			if(pid == 0) //children
			{
				close(sockfd);
				handle_socket(sockfd2);
				exit(0);
			}
			else close(sockfd2);//parents
		}
	}
	if(pid>0)wait(NULL);
	return 0;    
}

