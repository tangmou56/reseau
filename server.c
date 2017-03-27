#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/


#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0
#define SERVER_PORT 1500
#define MAX_MSG 100


int main(int argc,char *argv[]){
	int sd,newSd,cliLen;
	struct sockaddr_in cliAddr,servAddr;
	char line[MAX_MSG];
    int n;
    char rcv_msg[MAX_MSG];

	/*Creat Socket*/
	sd=socket(AF_INET,SOCK_STREAM,0);
	 if(sd<0){
		perror("cannot open socket");
		return ERROR;	
	}



	/* bind server port*/
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);
	

	if(bind(sd,(struct sockaddr *)&servAddr,sizeof(servAddr))<0){
		perror("cannot bind port");
		return ERROR;	
	}

	listen(sd,5);
	  while(1){
		printf("%s : waiting for data on port TCP %u\n",argv[0],SERVER_PORT);
		cliLen = sizeof(cliAddr);
		newSd= accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
		if(newSd<0){
			perror("cannot accept connection");
			return ERROR;			
		}		

	/*recive segments*/
          
	memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
          
	while( recv(newSd,rcv_msg,MAX_MSG,0)  > 0)/* wait for data */
        printf("%s : received from %s :TCP%d : %s\n",argv[0],inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),rcv_msg);
	
	/*init line*/
	memset(rcv_msg,0x0,MAX_MSG);
          
	  }/*while(1)*/

}
