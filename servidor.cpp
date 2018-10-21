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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */

int main(){
        printf("Iniciando servidor...\n");
        char str[100];
        int listen_fd, comm_fd, port;
        int port_validation = 0;

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

        // define porta do serviço do servidor
        while(port_validation ==0){
                printf("Defina porta do servidor: ");
                scanf("%d", &port);
                // se porta reservada for escolhida, repete rotina até porta ser válida
                if (port <=1023)
                {
                        printf("Porta reservada. Por favor, escolha uma porta com valor maior que 1024.\n");
                }else  {
                        port_validation++;
                        printf("- - - - -\nEsperando conexão na porta %d\n", port);
                }
        }

        // define porta de escuta
        servaddr.sin_port = htons(port);

        int listen_sock;
        if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
                printf("Falha na criação da socket para escuta...\n");
                return 1;
        }

        //printf("Porta: %d\nEsperando conexão...\n", port);

        // define os atributos para a escuta
        if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
                printf("Falha no bind... \n");
                return 1;
        }

        // inicia a escuta e limita a quantidade de conexões em espera. Se conxões maior que limite, conexão falha
        if(listen(listen_fd, 10) < 0){
                printf("Não foi possível criar socket para escuta...\n");
                return 1;
        }

        struct sockaddr_in client_address;
        int client_address_len = 0;

        while(true){
                int sock;

                // espera e aceita conexão
                if ((comm_fd = accept(listen_fd, (struct sockaddr*)&client_address, NULL)) < 0){
                        printf("Não foi possível abrir socket.\n");
                        return 1;
                }

                int n = 0;
                int len = 0, maxlen = 100;
                char buffer[maxlen];
                char *pbuffer = buffer;

                printf("Cliente conectado com IP: %s\n", inet_ntoa(client_address.sin_addr));

                while((n = recv(sock, pbuffer, maxlen, 0)) > 0){
                        pbuffer += n;
                        maxlen -= n;
                        len += n;

                        bzero( str, 100);
                        read(comm_fd,str,100);
                        printf("Reenviando mensagem: %s",str);
                        write(comm_fd, str, strlen(str)+1);
                }
                close(sock);

        }
        close(listen_sock);
        return 0;
}
//falta atualzar o client
