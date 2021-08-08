#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


int main(int argc,char** argv)
{
    if(argc < 3)
    {
        fprintf(stderr,"Not Enough Parameters ...\n");
        exit(EXIT_FAILURE);
    }
    
    int serverSocketDescripteur,structLen,result,BytesSended=0,FileLenght,SIZEMAX = 65000,PacketNumber=0;
    struct sockaddr_in clientSocket;
    char FileInfos[20],*buffer;
    FILE *file;

    memset(&clientSocket,'0',sizeof(clientSocket));

    clientSocket.sin_family = AF_INET;
    clientSocket.sin_port = htons(atoi(argv[2]));
    clientSocket.sin_addr.s_addr = inet_addr(argv[1]);

    if((serverSocketDescripteur = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,"Failed to Create the Socket ...\n");
        exit(EXIT_FAILURE);
    }

    if(connect(serverSocketDescripteur, (struct sockaddr *)&clientSocket,sizeof(clientSocket)) == -1)
    {
        fprintf(stderr,"Failed to Connect to The Server ...\n");
        exit(EXIT_FAILURE);
    }

    if((file = fopen(argv[3],"r")) == NULL)
    {
        fprintf(stderr,"Failed to Open The File ...\n");
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_END);
    FileLenght = ftell(file);
    fseek(file,0,SEEK_SET);
    buffer = (char*)malloc(SIZEMAX * sizeof(char));
    sprintf(FileInfos,"%s@%d",argv[3],FileLenght);
    
    if(send(serverSocketDescripteur,FileInfos,sizeof(FileInfos),0) == 0)
    {
        fprintf(stderr,"Failed to Send The File Infos To The Server ...\n");
        exit(EXIT_FAILURE);
    }

    while(BytesSended < FileLenght)
    {
        if(FileLenght-BytesSended < SIZEMAX)
        {
            SIZEMAX = FileLenght-BytesSended;
            buffer = realloc(buffer,SIZEMAX);
        }

        if((result = fread(buffer,1,SIZEMAX,file)) == -1)
        {
            fprintf(stderr,"Failed to Read From The File ...\n");
            exit(EXIT_FAILURE);
        }

        if((result = send(serverSocketDescripteur,buffer,SIZEMAX,0)) == -1)
        {
            fprintf(stderr,"Failed to send Data to Server ...\n");
            exit(EXIT_FAILURE);
        }

        printf("Sending %d bytes ...\n",result);
        BytesSended += result;
        PacketNumber++;
    }
    printf("Bytes Sended : %d  ,  File Lenght :  %d \n",BytesSended,FileLenght);
    printf("Number of Packet Sended is %d ...\n",PacketNumber);
    printf("File Sended Successfully ...\n");
    close(serverSocketDescripteur);
    close(serverSocketDescripteur);
    fclose(file);
    return 0;
}