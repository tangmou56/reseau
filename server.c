#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/
#include <string.h>
#include <stdlib.h>
#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0
#define SERVER_PORT 1502
#define MAX_MSG 100


void print_sockaddr(struct sockaddr_in n){
	printf("sin_family: %i\nsin_port: %i\nsin_addr: %s\n",n.sin_family,ntohs(n.sin_port), inet_ntoa(n.sin_addr));
}

void creer_fichier(char *donne,int n){
	char nb[20];
	
	sprintf(nb, "%d", n);
	char nom[20]="data_recv";
	strcat(nom,nb);
	FILE *fic;
	fic=fopen(nom,"w");
	fprintf(fic,"%s",donne);
	fclose(fic);
	printf("fichier %s enregistre\n",nom);
}







int main(int argc,char *argv[]){
	int sd,newSd,cliLen;
	struct sockaddr_in cliAddr,servAddr;
	char donne[MAX_MSG];
    char rcv_msg[MAX_MSG];
	int rc;
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
	printf("%s : waiting for data on port TCP %u\n",argv[0],SERVER_PORT);
	cliLen = sizeof(cliAddr);
	newSd= accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
	if(newSd<0){
		perror("cannot accept connection");
		return ERROR;
	}
	  while(1){
		  printf("waiting for data...\n");
		  memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		  recv(newSd,rcv_msg,MAX_MSG,0);/* wait for data */
		  printf("message recu:%s\n",rcv_msg);
		  /*init line*/
		  memset(rcv_msg,0x0,MAX_MSG);
		  printf("Saisir le message : ");
		  scanf("%s",donne);
		  if(!strcmp(donne,"q")){
			  close(sd);
			  exit(1);
		  }
		  rc=send(newSd,donne,strlen(donne)+1,0);
		  if(rc<0){
			  perror("cannot send date");
			  close(newSd);
			  exit(1);
		  }
	  }/*while(1)*/

}
