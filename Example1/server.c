#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include<string.h>
void toUpperCase(char* ch)
{
    char *p;
    for(p=ch;*p!='\0';p++)
    {
        if(*p >= 'a' && *p <= 'z')
        {
            *p = *p -32;
        }
    }
}

int main(int argc, char** argv) {
  if(argc < 1)
  {
      fprintf(stderr,"Not Enough Parameters ...\n");
      exit(EXIT_FAILURE);
  }
  
  struct sockaddr_un SocketAddress;
  int socketDescripteur,clientDescripteur;
  char message[20];

  if ( (socketDescripteur = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr,"Failed Create The Socket ...\n");
    exit(EXIT_FAILURE);
  }

  memset(&SocketAddress, 0, sizeof(SocketAddress));
  SocketAddress.sun_family = AF_UNIX;
  bcopy(argv[1],SocketAddress.sun_path,sizeof(argv[1]));
  
  if (bind(socketDescripteur, (struct sockaddr*)&SocketAddress, sizeof(SocketAddress)) == -1) {
    fprintf(stderr,"Failed To Bind ...\n");
    exit(EXIT_FAILURE);
  }

  if (listen(socketDescripteur, 5) == -1) {
    fprintf(stderr,"Failed To listen ...\n");
    exit(EXIT_FAILURE);
  }

  if ( (clientDescripteur = accept(socketDescripteur, NULL, NULL)) == -1) {
      fprintf(stderr,"Failed To accept a new Client ...\n");
      exit(EXIT_FAILURE);
    }

  if(recv(clientDescripteur,message,sizeof(message),0) == -1)
  {
     fprintf(stderr,"Failed To Receive The Message ...\n");
      exit(EXIT_FAILURE);
  }

  printf("Message Received : %s \n",message);
  toUpperCase(message);

  if(send(clientDescripteur,message,sizeof(message),0) == -1)
  {
    fprintf(stderr,"Failed To Send The Message ...\n");
      exit(EXIT_FAILURE);
  }
  close(socketDescripteur);
  close(clientDescripteur);
  return 0;
}