#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

void getFileInfos(char* message,char* fileN,int *fileL)
{
    char *token = strtok(message,"@");
    strcpy(fileN,token);
    token = strtok(0,"@");
    *fileL = atoi(token);
}


int main(int argc,char** argv)
{
     if(argc < 1)
    {
        fprintf(stderr,"Not Enough Parameters ...\n");
        exit(EXIT_FAILURE);
    }
    int socketDescripteur,clientSocketDescripteur,result,BytesReceived=0,structlen,FileLenght,SIZEMAX = 65000,PacketNumber=0;
    struct sockaddr_in serverSocket;
    char FileInfos[20],FileName[10],Path[10],*buffer;
    FILE *file;

    memset(&serverSocket,'0',sizeof(serverSocket));

    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(atoi(argv[1]));
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    if((socketDescripteur = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,"Failed to create The Socket ...\n");
        exit(EXIT_FAILURE);
    }

    if(bind(socketDescripteur,(struct sockaddr*)&serverSocket,sizeof(serverSocket)) == -1)
    {
        fprintf(stderr,"Failed to Bind ...\n");
        exit(EXIT_FAILURE);
    }
    
    if(listen(socketDescripteur,10) == -1)
    {
        fprintf(stderr,"Failed to listen ...\n");
        exit(EXIT_FAILURE);
    }
    
    if((clientSocketDescripteur = accept(socketDescripteur,(struct sockaddr*)NULL,NULL)) == -1)
    {
        fprintf(stderr,"Failed to accept a new Client ...\n");
        exit(EXIT_FAILURE);
    }

   if(recv(clientSocketDescripteur,FileInfos,sizeof(FileInfos),0) == -1)
   {
       fprintf(stderr,"Failed to Receive The File Infos from the Client ...\n");
       exit(EXIT_FAILURE);
   }

    getFileInfos(FileInfos,FileName,&FileLenght);
    sprintf(Path,"files/%s",FileName);

    struct stat st;
    memset(&st,'0',sizeof(st));
    if(stat("files",&st) == -1)
    {
        mkdir("files",0700);
    }

    buffer = (char*)malloc(SIZEMAX*sizeof(char));

    if((file = fopen(Path,"w")) == NULL)
    {
        fprintf(stderr,"Failed to Open The File ...\n");
        exit(EXIT_FAILURE);
    }

    while (BytesReceived < FileLenght)
    {
        if(FileLenght - BytesReceived < SIZEMAX)
        {
            SIZEMAX = FileLenght - BytesReceived;
            buffer = realloc(buffer,SIZEMAX);
        }

        if((result = recv(clientSocketDescripteur,buffer,SIZEMAX,0)) == -1)
        {
            fprintf(stderr,"Failed to from The Client ...\n");
            exit(EXIT_FAILURE);
        }

        printf("Receiving %d bytes ... \n",result);

        if((result = fwrite(buffer,1,SIZEMAX,file)) == -1)
        {
            fprintf(stderr,"Failed to Write in the File ...\n");
            exit(EXIT_FAILURE);
        }
        BytesReceived += result;
        PacketNumber ++;
    }

    printf("Bytes Received : %d  , FileLenght : %d ... \n",BytesReceived,FileLenght);
    printf("Number of Packet Received is %d ... \n",PacketNumber);
    printf("File Received Successfully ...\n");
    close(socketDescripteur);
    close(socketDescripteur);
    fclose(file);
    return 0;
}
