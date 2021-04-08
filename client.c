#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
//Constantes
#define mbytes 1024

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("[ERRO] - Use: %s HOST PORTA ARQUIVO\n", argv[0]);
        return EXIT_FAILURE;
    }
    int PORTA = atoi(argv[2]);
    char *hostname = argv[1];
    int skt;
    char buffer[mbytes];
    FILE *transferfile;
    char *namefile = argv[3];
    struct sockaddr_in client;

    client.sin_family       = AF_INET;
    client.sin_port         = htons(PORTA);
    client.sin_addr.s_addr  = inet_addr(hostname);

    skt = socket(AF_INET, SOCK_STREAM, 0);

    connect(skt, (struct sockaddr *)&client, sizeof(client));

    transferfile = fopen(argv[3], "r");
    while(fgets(buffer, sizeof(buffer), transferfile)){
        if(send(skt, buffer, sizeof(buffer), 0) == -1){
            fprintf(stderr, "[ERRO] Erro ao enviar o arquivo.\n");
            return EXIT_FAILURE;
        }
        memset(&buffer,'\0',sizeof(buffer));
    }
    printf("Arquivo enviado com sucesso!");
    close(skt);
    return 0;
}

