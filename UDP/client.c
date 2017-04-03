#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/
#include <string.h>
#include <stdlib.h>


#define MAX_MSG 100


int main(int argc,char *argv[]){
	int sd,rc;
	struct sockaddr_in localAddr,servAddr;
	struct hostent *h;
	char donne[MAX_MSG];
	char rcv_msg[MAX_MSG];
	if(argc<3){
		printf("usage %s <server> <port>\n",argv[0]);
		exit(1);
		

	}
	int SERVER_PORT=atoi(argv[2]);
	h=gethostbyname(argv[1]);
	if(h==NULL){
		printf("%s unknown host '%s'\n",argv[0],argv[1]);
		exit(1);	
	}
	servAddr.sin_family = h->h_addrtype;
	memcpy((char *)&servAddr.sin_addr.s_addr,h->h_addr_list[0],h->h_length);
	servAddr.sin_port=htons(SERVER_PORT);
	
	/* creat socket*/
	sd = socket(PF_INET,SOCK_DGRAM,0);
	if(sd<0){
		perror("cannot open socket");	
		exit(1);	
	}
	
	/*bind any port number*/
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	localAddr.sin_port=htons(0);

	rc=bind(sd,(struct sockaddr *)&localAddr,sizeof(localAddr));
	if(rc<0){
		printf("%s : cannot bind port UDP %u\n",argv[0],SERVER_PORT);
		perror("error");
		exit(1);	
	}	

	/*connect to server*/

	while(1){
		printf("Saisir le message : ");
		scanf("%s",donne);
		rc=sendto(sd,donne,strlen(donne)+1,0,(struct sockaddr*)&servAddr,sizeof(servAddr));
	}
	
	
		return 0;

}/* main */
