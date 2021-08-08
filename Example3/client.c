#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
 
int main(void)
{
  int sockdescripteur = 0;
  char receivedMessage[1024],messageToSend[100];
  struct sockaddr_in serv_addr;
 
  memset(receivedMessage, '0' ,sizeof(receivedMessage));
  memset(messageToSend,'0',sizeof(messageToSend));
  if((sockdescripteur = socket(AF_INET, SOCK_STREAM, 0))< 0)
  {
      perror("Failed to create The Socket ...\n");
      exit(EXIT_FAILURE);
  }
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
  if(connect(sockdescripteur, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      perror("Connection Failed ...\n");
      exit(EXIT_FAILURE);
    }
      printf("Enter The Command => ");
      gets(messageToSend);
      if(send(sockdescripteur,messageToSend,sizeof(messageToSend),0) == -1)
      {
        perror("Failed to send the command ...\n");
        exit(EXIT_FAILURE);
      }

      if(recv(sockdescripteur,receivedMessage,sizeof(receivedMessage),0) == -1)
      {
        perror("Failed to receive the result  ...\n");
        exit(EXIT_FAILURE);
      }
      printf("%s",receivedMessage);
 
  return 0;
}
