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
    if(argc < 2){
        printf("[ERRO] - Use: %s PORTA ARQUIVO\n", argv[0]);
        return EXIT_FAILURE;
    }
    int PORTA = atoi(argv[1]);
    int skt, acptskt;
    char buffer[mbytes];
    FILE *transferfile;
    char *namefile = argv[2];
    struct sockaddr_in svaddr,client;
    socklen_t sizeclient = sizeof(client);
    svaddr.sin_family       = AF_INET;
    svaddr.sin_port         = htons(PORTA);
    svaddr.sin_addr.s_addr  = htons(INADDR_ANY);

    if( (skt = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("[ERRO] Erro na criação de socket");
        return EXIT_FAILURE;
    }
    if( (bind(skt, (struct sockaddr *)&svaddr, sizeof(svaddr))) < 0){
        printf("[ERRO] Erro na criação no bind");
        return EXIT_FAILURE;
    }
    listen(skt, 2);

    acptskt = accept(skt, (struct sockaddr *)&client, &sizeclient);

    transferfile = fopen(argv[2], "w");

    while ( (recv(acptskt,buffer,mbytes,0)) > 0 ){
       printf("%s", buffer);
       fprintf(transferfile, "%s", buffer);
       memset(&buffer,'\0',sizeof(buffer));
    }
    printf("Arquivo recebido!\nSalvo como: %s",);
    close(skt);
    return 0;
}
