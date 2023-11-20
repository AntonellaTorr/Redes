
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "common.h"

#define MSG_SIZE sizeof(struct infoMessage)


int sock;

int init(int port, char *server_name)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server = gethostbyname(server_name);

    if (server == NULL)
        error(1, 0, "Servidor %s no encontrado.\n", server_name);

    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    printf("Connecting to %s:%d...\n", server->h_name, port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // Conectamos con el servidor
        error(1, 0, "No se puede conectar.\n");

    return sock;
}

int main(int argc, char *argv[])
{
    struct infoMessage msg;
    struct move m;


    if (argc < 3)
        error(1, 0, "Ingrese el numero de puerto y el servidor\n");

    sock = init(atoi(argv[1]), argv[2]);

    recv_message(sock, &msg);

    while (msg.gameState)
    {
        //si es mi turno
        if (msg.turnPlayer == msg.playerId)
        {
            //mientras que n tuve exito
            while (msg.moveSuccess==0)
            {
              
                // Solicita al usuario que ingrese el primer número
                printf("Ingrese la fila: \n");
                scanf("%d", &m.row);

                // Solicita al usuario que ingrese el segundo número
                printf("Ingrese la columna: \n");
                scanf("%d", &m.column);

                // Imprime los números ingresados
                printf("Has ingresado los números: %d y %d\n", m.row, m.column);
                sendMove(sock, &m);

                //Recibe ok del server
                recv_message(sock, &msg);
                
             
                if (!msg.moveSuccess)
                {
                    printf("ERROR-JUGADA INVALIDA\n");
                }
                else{
                    printTable(msg.table);
                } 
            }
        }
        else { 

            printf("No es mi turno, espero \n");
            while (msg.turnPlayer!=msg.playerId) {
                recv_message(sock, &msg);
            }  
            printTable(msg.table);
        }  
    }


    switch (msg.winner){
        case 0: printf ("EMPATE\n"); break;
        case 1: printf ("Ganador jugador 1\n"); break;
        case 2: printf ("Ganador jugador 2\n"); break;
    }


}
