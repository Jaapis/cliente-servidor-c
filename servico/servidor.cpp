/*
Title: Servidor
Date: 20/10/2018
Author: Rodrigo Toshiaki Horie - 26620
*/

//includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */

int main()
{
        printf("Iniciando servidor...\n");
        char str[100];
        int listen_fd, comm_fd;

        // especifica IP e Porta
        struct sockaddr_in servaddr;

        // escuta conexões e cria um socket a partir dela
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);

        // zera valores de memória
        bzero( &servaddr, sizeof(servaddr));
        // define protocolo IP para conexão
        servaddr.sin_family = AF_INET;
        // permite qualquer conexão IP
        servaddr.sin_addr.s_addr = htons(INADDR_ANY);
        // define porta de escuta
        servaddr.sin_port = htons(9000);

        printf("Porta: 9000\nEsperando conexão...\n");

        // define os atributos para a escuta
        bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        // inicia a escuta e limita a quantidade de conexões em espera. Se conxões maior que limite, conexão falha
        listen(listen_fd, 5);

        // espera e aceita conexão
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

        printf("Conexão estabelecida! Aguardando mensagem...\n");

        while(1)
        {
                bzero( str, 100);
                read(comm_fd,str,100);
                printf("Reenviando mensagem... - %s",str);
                write(comm_fd, str, strlen(str)+1);

                if (comm_fd > 0)
                {
                        handleClient(comm_fd)
                }
        }
        return 0;
}

void handleClient(int sd)
{
   fd_set read_sd;
   FD_ZERO(&read_sd);
   FD_SET(sd, &read_sd);

   while (true) {
      fd_set rsd = read_sd;

      int sel = select(sd + 1, &rsd, 0, 0, 0);

      if (sel > 0) {
         // client has performed some activity (sent data or disconnected?)

         char buf[1024] = {0};

         int bytes = recv(sd, buf, sizeof(buf), 0);

         if (bytes > 0) {
            // got data from the client.
         }
         else if (bytes == 0) {
            // client disconnected.
            break;
         }
         else {
            // error receiving data from client. You may want to break from
            // while-loop here as well.
         }
      }
      else if (sel < 0) {
         // grave error occurred.
         break;
      }
   }

   close(sd);
}
