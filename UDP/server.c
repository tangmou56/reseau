#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0

#define MAX_MSG 100


void print_sockaddr(struct sockaddr_in n){
	printf("sin_family: %i\nsin_port: %i\nsin_addr: %s\n",n.sin_family,ntohs(n.sin_port), inet_ntoa(n.sin_addr));
}







int main(int argc,char *argv[]){
	int sd;
	struct sockaddr_in cliAddr,servAddr;
	char donne[MAX_MSG];
   	char rcv_msg[MAX_MSG];
	int rc;
 	socklen_t len;
 	len=sizeof(cliAddr);
	/*Creat Socket*/
	sd=socket(PF_INET,SOCK_DGRAM,0);

	 if(sd<0){
		perror("cannot open socket");
		return ERROR;	
	}

	if(argc<2){
		printf("usage %s <port>\n",argv[0]);
		exit(1);
		

	}

	int SERVER_PORT=atoi(argv[1]);
	/* bind server port*/
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);

	

	if(bind(sd,(struct sockaddr *)&servAddr,sizeof(servAddr))<0){
		perror("cannot bind port");
		return ERROR;	
	}


	/*init line*/


	  while(1){
		printf("%s : waiting for data on port UDP %u\n",argv[0],SERVER_PORT);
		memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		rc=recvfrom(sd,rcv_msg,MAX_MSG,0,(struct sockaddr*)&cliAddr,&len);/* wait for data */
		if(rc>0)	  
			printf("message recu depuis %s : %s\n",inet_ntoa(cliAddr.sin_addr),rcv_msg);
	  }/*while(1)*/
	
	return 0;

}
