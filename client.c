#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/
#include <string.h>
#include <stdlib.h>

#define SERVER_PORT 1502
#define MAX_MSG 100


void lire(char *nom,char *donne){

	
	FILE *fp;
	fp = fopen(nom , "rb");
	fseek( fp , 0 , SEEK_END );
	int file_size = ftell(fp);
	fseek( fp , 0 , SEEK_SET);
	fread( donne , file_size , sizeof(char) , fp);
	donne[file_size] = '\0';
	fclose(fp);
}


void print_sockaddr(struct sockaddr_in n){
	printf("sin_family: %i\nsin_port: %i\nsin_addr: %s\n",n.sin_family,ntohs(n.sin_port), inet_ntoa(n.sin_addr));
}


int main(int argc,char *argv[]){
	int sd,rc;
	struct sockaddr_in localAddr,servAddr;
	struct hostent *h;
	char donne[MAX_MSG];
	char rcv_msg[MAX_MSG];
	if(argc<2){
		printf("usage %s <server>\n",argv[0]);
		exit(1);
		

	}
	h=gethostbyname(argv[1]);
	if(h==NULL){
		printf("%s unknown host '%s'\n",argv[0],argv[1]);
		exit(1);	
	}
	servAddr.sin_family = h->h_addrtype;
	memcpy((char *)&servAddr.sin_addr.s_addr,h->h_addr_list[0],h->h_length);
	servAddr.sin_port=htons(SERVER_PORT);
	
	/* creat socket*/
	sd = socket(AF_INET,SOCK_STREAM,0);
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
		printf("%s : cannot bind port TCP %u\n",argv[0],SERVER_PORT);
		perror("error");
		exit(1);	
	}	

	/*connect to server*/
	rc=connect(sd,(struct sockaddr *)&servAddr,sizeof(servAddr));
	if(rc<0){
		perror("cannot connect");
		exit(1);
	}

	while(1){
		printf("Saisir le message : ");
		scanf("%s",donne);
		if(!strcmp(donne,"q")){
			close(sd);
			exit(1);
		}
		rc=send(sd,donne,strlen(donne)+1,0);
		
		
		if(rc<0){
			perror("cannot send date");
			close(sd);
			exit(1);
		}
		
		
		memset(rcv_msg,0x0,MAX_MSG);
		printf("waiting for data...\n");
		recv(sd,rcv_msg,MAX_MSG,0);/* wait for data */
		printf("message recu:%s\n",rcv_msg);
		/*init line*/
		memset(rcv_msg,0x0,MAX_MSG);
			
		
	}



		return 0;

}/* main */
