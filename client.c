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


int gagne(char rec[],char send[]){
	/*1-pierre 2-feuille 3-ciseau*/ 
	int reci=atoi(rec);
	int sendi=atoi(send);
	if(reci==sendi){
		printf("L'autre joueur joue la meme\n");
		printf("Null!\n");
		return 0;	
	}
	if(sendi==1){
		if(reci==2){
			printf("L'autre joueur joue feuille\n");
			printf("Raté!\n");
			return -1;
		}
		if(reci==3){
			printf("L'autre joueur joue ciseau\n");
			printf("Gagné!\n");
			return 1;
		}
		
	}
	if(sendi==2){
		if(reci==3){
			printf("L'autre joueur joue ciseau\n");
			printf("Raté!\n");
			return -1;
		}
		if(reci==1){
			printf("L'autre joueur joue pierre\n");
			printf("Gagné!\n");
			return 1;
		}
		
	}
	if(sendi==3){
		if(reci==1){
			printf("L'autre joueur joue pierre\n");
			printf("Raté!\n");
			return -1;
		}
		if(reci==2){
			printf("L'autre joueur joue feuille\n");
			printf("Gagné!\n");
			return 1;
		}
		
	}	
}


int main(int argc,char *argv[]){
	int sd,rc,n,jeu=0,choix=0,point=0,tour=0;
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

	for(n=0;n<10;n++){
		  memset(rcv_msg,0x0,MAX_MSG);
		  recv(sd,rcv_msg,MAX_MSG,0);
		  printf("message recu:%s\n",rcv_msg);
		 
		  memset(rcv_msg,0x0,MAX_MSG);
	}


	printf("Saisir 'jeu' pour commencer le jeu. \n");
	while(jeu==0){
		printf("Saisir le message : ");
		scanf("%s",donne);
		if(!strcmp(donne,"jeu")){
			jeu=1;
		}
		rc=send(sd,donne,strlen(donne)+1,0);
		
		if(jeu==0){
			memset(rcv_msg,0x0,MAX_MSG);
			printf("waiting for data...\n");
			recv(sd,rcv_msg,MAX_MSG,0);/* wait for data */
			printf("message recu:%s\n",rcv_msg);
			/*init line*/
			memset(rcv_msg,0x0,MAX_MSG);
		}	
		
	}
	
	printf("Le jeu est commencé ");
	while(jeu==1){
		while(choix>3||choix<1){
			printf("chosir : 1-pierre 2-feuille 3-ciseau :");
			scanf("%i",&choix);
		}
		sprintf(donne, "%d", choix);
		rc=send(sd,donne,strlen(donne)+1,0);
		memset(rcv_msg,0x0,MAX_MSG);
		printf("waiting for another player...\n");
		recv(sd,rcv_msg,MAX_MSG,0);/* wait for data */
		point=point+gagne(rcv_msg,donne);
		tour++;
		if(tour==3)
			jeu=0;
		choix=0;
	}
	printf("Jeu fini\n");
	if(point>0)
		printf("Vous êtes gagné\n");
	else if(point<0)
		printf("Vous êtes raté\n");
	else
		printf("Jeu null\n");
	strcpy(donne,"fini");
	rc=send(sd,donne,strlen(donne)+1,0);

		return 0;

}/* main */
