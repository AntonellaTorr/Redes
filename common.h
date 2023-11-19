#include <string.h>
#include <bits/string.h>
#include <unistd.h>
#define MSG_STATE_SIZE 6
#define MSG_BOARD_SIZE 10
#define MSG_SIZE sizeof(struct infoMessage)
#define MV_SIZE sizeof(struct move)



//mensaje para mandarle al server 
struct move {
    int row;
    int column;
};
struct  infoMessage    {
    int gameState;  // estado del juego: iniciado 1,  finalizado 0 
    int table[3][3]; //la matrizzz
    int turnPlayer; //de quien es el turno, jugador 1 o 2
    int playerId;// id de los players
    int winner;
    int moveSuccess;
} ;

void sendMessage(int socketDest, struct infoMessage* msg){
    //le manda el msg al jugador 
    write(socketDest, msg, MSG_SIZE);
    printf("mande mensajitooo  al jugador");
    printf("\n");
}

void sendMove(int socketDest, struct move* m)
{
    //MANDA MSJ AL server
    write(socketDest, m, MV_SIZE);
    printf("mande mensajitooo de movimiento al servidor  ");
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
    read(from_sockfd, msg, MSG_SIZE);

}

/*
int message_state_is(struct message_t* msg, char* state)
{
    return !(strcmp(msg->state, state)!=0);
}*/