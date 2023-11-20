#ifndef COMMON_H
#define COMMON_H
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
void sendMessage(int socketDest, struct infoMessage* msg);
void sendMove(int socketDest, struct move* m);

//metodo del servidor void recv_move_fuser(int from_sockfd, struct move* m);
void printTable (int table[3][3] );
void recv_message(int from_sockfd, struct infoMessage* msg);

#endif