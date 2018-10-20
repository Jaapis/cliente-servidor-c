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
#include<string.h>

/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */

int main(int argc, char *argv[])
{
        char str[40000];
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

        // define os atributos para a escuta
        bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        // inicia a escuta e limita a quantidade de conexões em espera. Se conxões maior que limite, conexão falha
        listen(listen_fd, 5);

        // espera e aceita conexão
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

        while(1)
        {

                bzero( str, 40000);

                read(comm_fd,str,40000);

                printf("Reenviando mensagem... - %s",str);

                write(comm_fd, str, strlen(str)+1);

                }
        }
}
