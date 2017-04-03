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
	int rc,n,jeu=0,choix=0;

	struct  timeval start;
        struct  timeval end;
	unsigned  long diff;
	/*Creat Socket*/
	sd=socket(AF_INET,SOCK_STREAM,0);
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

	listen(sd,5);
	printf("%s : waiting for data on port TCP %u\n",argv[0],SERVER_PORT);
	cliLen = sizeof(cliAddr);
	newSd= accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
	if(newSd<0){
		perror("cannot accept connection");
		return ERROR;
	}

	/*init line*/
	printf("envoyer 10 nombres:\n");
	for(n=0;n<10;n++){
		printf("Nombre envoyé %i : ",n);
		sprintf(donne, "%d", rand()%49);
		gettimeofday(&start,NULL);
		rc=send(newSd,donne,strlen(donne)+1,0);
		gettimeofday(&end,NULL);
		diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		printf(" %ld micro second\n",diff);
		

		memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		gettimeofday(&start,NULL);
		recv(newSd,rcv_msg,MAX_MSG,0);/* wait for data */
		gettimeofday(&end,NULL);
		memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
		printf("reponse reçu : %ld micro second\n",diff);
		sleep(1);
	}
		

	  while(jeu==0){
		  printf("waiting for data...\n");
		  memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		  recv(newSd,rcv_msg,MAX_MSG,0);/* wait for data */
		  printf("message recu:%s\n",rcv_msg);
		  if(!strcmp(rcv_msg,"jeu")){
			jeu=1;
		  }
		if(jeu==0){
			  /*init line*/
			  memset(rcv_msg,0x0,MAX_MSG);
			  sprintf(donne, "%d", rand()%49);
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
		}
	  }/*while(jeu==0)*/
	printf("Jeu commence\n");
	while(jeu==1){
		sprintf(donne, "%d", rand()%2+1);
		rc=send(newSd,donne,strlen(donne)+1,0);
		printf("waiting for another player...\n");
		memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
		recv(newSd,rcv_msg,MAX_MSG,0);/* wait for data */
		printf("message recu:%s\n",rcv_msg);
		if(!strcmp(rcv_msg,"fini"))
			jeu=0;
	}
	printf("Jeu fini\n");
	return 0;

}
