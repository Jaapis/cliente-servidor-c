/*
Cliente
Autor: Rodrigo Toshiaki Horie - 26620
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc , char *argv[])
{
        int sock, port, i=0;
        struct sockaddr_in server;
        char message[40000] , server_reply[40000], ip_addr[16];
        char fim[] = "tchau\n";

        printf("Insira IP do server:");
        scanf("%s", ip_addr);

        printf("Insira a porta do server:");
        scanf("%d", &port);

        //Cria socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
                printf("Não foi possível criar o socket");
        }
        puts("Socket criado");

        server.sin_addr.s_addr = inet_addr(ip_addr);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        //Conecta ao servidor
        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
                perror("Falha na conexão");
                return 1;
        }

        puts("Conectado!\n");

        bzero(message, 40000);
        bzero(server_reply, 40000);
        fgets(message,40000,stdin);


        //Loop da conexão do cliente
        while(i == 0)
        {
                printf("Client: ");
                //scanf("%s" , message);
                bzero(message, 40000);
                bzero(server_reply, 40000);
                fgets(message,40000,stdin);

                write(sock,message,strlen(message)+1);
                read(sock,server_reply,40000);
                printf("Server: %s",server_reply);

                if (strcmp(fim, message) == 0) {
                        i++;
                        printf("Saindo do programa...");
                }
        }

        close(sock);
        return 0;
}
