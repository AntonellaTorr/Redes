#include <string.h>
//#include <bits/string.h>
#include <unistd.h>
#include <stdio.h>
#include "common.h"

void sendMessage(int socketDest, struct infoMessage* msg){
    //le manda el msg al jugador 
    write(socketDest, msg, MSG_SIZE);
    printf("\n");
}

void sendMove(int socketDest, struct move* m)
{
    //MANDA MSJ AL server
    write(socketDest, m, MV_SIZE);
    printf("\n");
}  

//metodo del servidor 
void recv_move_fuser(int from_sockfd, struct move* m){
    read(from_sockfd, (void*)m, MV_SIZE);
} 

void printTable (int table[3][3] ) {
       printf("\n");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

}  

void recv_message(int from_sockfd, struct infoMessage* msg)
{
    read(from_sockfd,(void*)msg, MSG_SIZE);

}

