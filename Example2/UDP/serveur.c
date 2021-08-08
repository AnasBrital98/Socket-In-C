#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<unistd.h>
#include "structure.h"

void getEtudiant(char *str,Etudiant *e)
{
	int i=0;
	const char *delimiteur = "-"; 
	char* token; 
	  
    token = strtok(str, delimiteur); 
  
    while (token != 0) { 
       	switch (i)
	{
	case 0 :
		e->numero = atoi(token);
		break;
	case 1 :
		strcpy(e->prenom,token);
		break;
	case 2 :
		strcpy(e->nom,token);
		break;
	case 3 :
		strcpy(e->cne,token);
		break;
	case 4 :
		strcpy(e->filiere,token);
		break;
	}
	i++; 
        token = strtok(0, delimiteur); 
    }	
}



int main(int argc, char **argv){
	
	//déclaration des variables
	int ServerDescipteur,longSocketClient;
	char messageReceived[1024];
	Etudiant e;
	FILE *PF;
	struct sockaddr_un AddressServer,AddressClient;
	bzero((char*)&AddressServer,sizeof(AddressServer));	
	AddressServer.sun_family = AF_UNIX;
	bcopy("unix_socket",AddressServer.sun_path,11);
	if((ServerDescipteur = socket(AF_UNIX,SOCK_DGRAM,0)) == -1)
	{
	perror("Failed to Create Socket ... \n");
	exit(EXIT_FAILURE);
	}
	if(bind(ServerDescipteur,(struct sockaddr*)&AddressServer,sizeof(AddressServer)) == -1)
	{
		perror("Failed to Bind ... \n");
		exit(EXIT_FAILURE);
	}

	printf("The connection has been established successfully \n");	
	PF = fopen("file2.txt","w");
	if(PF == NULL)
	{
		perror("Failed to Open The file");
		exit(EXIT_FAILURE);
	}

    longSocketClient = sizeof(AddressClient);
	int i;
	for(i=0;i<10;i++)
	{
		recvfrom(ServerDescipteur,messageReceived,sizeof(messageReceived),0,(struct sockaddr *)&AddressClient,&longSocketClient);
		getEtudiant(messageReceived,&e);
		fprintf(PF,"%d\t%s\t%s\t%s\t%s\n",e.numero,e.prenom,e.nom,e.cne,e.filiere);	
	}
	close(ServerDescipteur);
	fclose(PF);
	unlink("unix_socket");
	return EXIT_SUCCESS;
}



