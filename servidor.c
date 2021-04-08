#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
//Constantes
#define mbytes 20024

int recv_data(int socket, char *namefile, int op);
int main(int argc, char *argv[]){
    int op = 0;
    if(argc < 3){
        printf("[ERRO] - Use: %s PORTA (OUTPUT)ARQUIVO\nPara transferencia de imagem Use -i", argv[0], argv[0]);
        return EXIT_FAILURE;
    }
    for(int i = 0;i < argc;i++){
        if (strcmp("-i", argv[i]) == 0) {
            op = 1;
       }
    }
    int PORTA = atoi(argv[1]);
    int skt, acptskt,check_recv;
    char *namefile = argv[2];
    struct sockaddr_in svaddr,client;
    socklen_t sizeclient = sizeof(client);
    svaddr.sin_family       = AF_INET;
    svaddr.sin_port         = htons(PORTA);
    svaddr.sin_addr.s_addr  = htons(INADDR_ANY);
    printf("Programa com o proposito de realizar\ntransferencias de arquivo (Imagens,Binarios,Texto)\nBy: Kanui\n");
    if( (skt = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("[ERRO] Erro na criação de socket");
        return EXIT_FAILURE;
    }
    if( (bind(skt, (struct sockaddr *)&svaddr, sizeof(svaddr))) < 0){
        printf("[ERRO] Erro na criação no bind");
        return EXIT_FAILURE;
    }
    printf("\nAguardando Conexao...\n");
    listen(skt, 2);

    acptskt = accept(skt, (struct sockaddr *)&client, &sizeclient);
    if(acptskt < 0){
        printf("[ERRO] Ocorreu um erro ao aceitar a conexao!");
        return 0;
    }
    check_recv = recv_data(acptskt, argv[2], op);

    if(check_recv == 0){
        printf("Arquivo recebido!\nSalvo como: %s",argv[2]);
    }
    close(skt);
    return 0;
}

int recv_data(int socket, char *namefile, int op){
    FILE *transferfile;
    int recv_size = 0,size = 0,status,write_s,ler_size;
    char servidormsg[] = "[SERVIDOR] A Conexao foi estabelecida!\n";
    char buffer[mbytes];

    status = recv(socket, &size, sizeof(size), 0);
    if(status < 0){
        printf("[ERRO] Ocorreu um erro ao receber o tamanho do arquivo!\n");
        return 0;
    }
    printf("Tamanho do arquivo: %d Bytes\n",size);

    status = send(socket, servidormsg, sizeof(servidormsg), 0);
    if(status < 0){
        printf("[ERRO] Ocorreu um erro no envio da mensagem:%s\n", servidormsg);
    }

    if( (transferfile = fopen(namefile, "w")) == NULL){
        printf("[ERRO] Ocorreu um erro na tentativa de escrita do arquivo\n");
        return EXIT_FAILURE;
    }

    while(size > recv_size){
        ler_size = recv(socket,buffer,sizeof(buffer), 0);
        if(recv_size < 0){
            printf("%s", strerror(errno));
        }
        if(op == 1)
            write_s = fwrite(buffer,1,sizeof(buffer), transferfile);
        else
            fprintf(transferfile, "%s", buffer);

        recv_size += ler_size;
    }

    fclose(transferfile);
    return 0;
}
