#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<unistd.h>
#include "structure.h"

void getString(char* result ,Etudiant e)
{
sprintf(result,"%d-%s-%s-%s-%s",e.numero,e.prenom,e.nom,e.cne,e.filiere);
}

int main(int argc, char **argv)
{

	//Declaration des Variables	
	int SocketDescripteur;
	Etudiant e;	
	FILE *PF;
	char messageToSend[100];
	
	struct sockaddr_un serverAddress;
	bzero((char*)&serverAddress,sizeof(serverAddress));
	serverAddress.sun_family = AF_UNIX;
	bcopy("unix_socket",serverAddress.sun_path,11);	
	if((SocketDescripteur  = socket(AF_UNIX,SOCK_DGRAM,0)) == -1)
	{
		perror("Failed to create Socket ...\n");
		exit(EXIT_FAILURE);
	}

	printf("The connection has been established successfully \n");

	PF = fopen("file1.txt","r");
	if(PF == NULL)
	{
		perror("Failed to Open The file");
		exit(EXIT_FAILURE);
	}

	while(!feof(PF))
	{		
	fscanf(PF,"%d\t%s\t%s\t%s\t%s\n",&e.numero,e.prenom,e.nom,e.cne,e.filiere);
	getString(messageToSend,e);	
	printf("message to send : %s\n",messageToSend);
	sendto(SocketDescripteur,messageToSend,sizeof(messageToSend),0,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
	}
    //sleep(100);
	fclose(PF);
	close(SocketDescripteur);	
	return EXIT_SUCCESS;
}