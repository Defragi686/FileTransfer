#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
//Constantes
#define mbytes 20024

int send_data(int socket, char *namefile);

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("[ERRO] - Use: %s HOST PORTA (INPUT)ARQUIVO\n[INFO] Exemplo:%s 127.0.0.1 4444 foto.jpg", argv[0],argv[0]);
        return EXIT_FAILURE;
    }
    int PORTA = atoi(argv[2]);
    char *hostname = argv[1];
    int skt,check_return;
    struct sockaddr_in client;
    printf("Programa feito em c com o proposito de realizar\ntransferencias de arquivo (Imagens,Binarios,Texto)\nBy: Kanui\n\n");
    client.sin_family       = AF_INET;
    client.sin_port         = htons(PORTA);
    client.sin_addr.s_addr  = inet_addr(hostname);

    skt = socket(AF_INET, SOCK_STREAM, 0);
    if(skt < 0){
        printf("[ERRO] Erro ao tentar criar socket!");
        return EXIT_FAILURE;
    }
    connect(skt, (struct sockaddr *)&client, sizeof(client));

    check_return = send_data(skt, argv[3]);
    if(check_return == 0){
        printf("[INFO] Arquivo enviado com sucesso!");
    }
    close(skt);
    return 0;
}

int send_data(int skt, char *namefile){
    char buffer[mbytes];
    FILE *transferfile;
    int size,read_size,status;
    char minbuffer[60];
    if( (transferfile = fopen(namefile, "r")) == NULL){
        printf("[ERRO] Ocorreu um erro ao tentar ler o arquivo...");
        return EXIT_FAILURE;
    }

    printf("[INFO] Obtendo o tamanho do arquivo...\n");

    sleep(3);

    fseek(transferfile, 0, SEEK_END); //Seta o indicador no final do arquivo
    size = ftell(transferfile);// Retorna o valor atual do indicador 
    fseek(transferfile, 0, SEEK_SET); //Seta o indicador no começo do arquivo

    printf("[INFO] Total de bytes do arquivo: %d\n",size);
    sleep(1);
    printf("[INFO] Realizando o envio de dados...\n");

    send(skt, &size,sizeof(size),0);
    recv(skt, minbuffer, sizeof(minbuffer), 0);
    printf("%s\n", minbuffer);
    sleep(2);
    while(!feof(transferfile))
    {
        read_size = fread(buffer,1, sizeof(buffer), transferfile);

        do{
            status = write(skt, buffer,read_size);
        }while(status < 0);

        bzero(buffer, sizeof(buffer));

    }
    fclose(transferfile);
    return 0;
    
}
