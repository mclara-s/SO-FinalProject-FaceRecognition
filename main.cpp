#include <errno.h>
//Bibliotecas socket
#include <sys/types.h>  //AF_INET
#include <string>
#include <sys/socket.h> //socket
#include <netinet/in.h> //htons
#include <arpa/inet.h> //inet_addr
#include <unistd.h>  //close
#include <iostream>
#include <fstream>
#include "packet2.h"
using namespace std;

/* configurações do IP e Porta do Servidor, IP do IMD */
static const char LG_IP[] = "10.7.204.2";
#define LG_PORTA 9002
#define DEBUG_MODE 1


/*Abrir arquivo com número de pessoas e calcular temperatura */
void getTemp(unsigned char temp[3]){
  int temperatura, command;
  ifstream file;
  string str;

  file.open("faces", ifstream::in);
  file >> str;
  temperatura = stoi(str);
  if (temperatura == 30)
    command = 0; // OFF
  else
    command = 1;  //ON

  temp[0] = str[0];
  temp[1] = str[1];
  temp[2] = '\0';
  /*
    int npessoas = stoi(str);
    float temp = 25.3;
    char command[4];
    if (npessoas == 0)
      command = "OFF\0";
    else{
      command = "ON\0";
      temp = 25.3 - (0.3)*npessoas
    }
  */
} 

int main(int argc, char *argv[])
{
      int socketId;
      struct sockaddr_in endereco;
      packet *Pacote = new packet();
      u8 *servP = new u8[50];
      unsigned char temp[3];
      int bytesLidos, count;

      socketId = socket(AF_INET, SOCK_STREAM, 0);
      if (socketId == -1)
      {
         perror("Error socket()\n");
         exit(1);
      }

      endereco.sin_family = AF_INET;
      endereco.sin_port = htons(LG_PORTA);  //MUDAR PARA LG_PORTA
      endereco.sin_addr.s_addr = inet_addr(LG_IP); // MUDAR PARA LG_IP
      bzero( &( endereco.sin_zero ), 8);
 
      if ( connect (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
      {
         perror("Error connect()\n");
         exit(1);
      }

      Pacote->packet_Login();
      Pacote->getServerPacket(servP);

     /*verificação se a cópia foi correta */
     printf ("PACOTE ENVIADO: \n");
     for (int i = 0; i < Pacote->getPacketCount(); i++)
        fprintf(stderr, " %X", servP[i]);

      
      int size = Pacote->getPacketCount();
      if ( send (socketId, servP, size, 0) == -1 )
            {
              perror ("Error send\n");
              exit(1);
            }

     while (true){
          memset(servP, 0x0, 50);
          //Recebendo um caracter do servidor
          if ( bytesLidos = recv (socketId, servP, size, 0) == -1 )
            {
              perror ("Error recv\n");
              exit(1);
            }

            printf("\nPACOTE RECEBIDO: \n");
          /*Imprimir pacote recebido */
          for (int i = 0; i < Pacote->getPacketCount(); i++)
            fprintf(stderr, " %X", servP[i]);

          printf("\n");
          /*Packet Handle*/
           Pacote->packetHandle();

          getTemp(temp);
          printf("Temperatura: %s\n", temp);
          //Pacote->setTemp(temp);
     
          /* Enviar novos dados */  
          if ( send (socketId, servP, size, 0) == -1 )
            {
              perror ("Error send\n");
              exit(1);
            }
          sleep(1);       
      }
      close (socketId);
 
    return 0;
  }