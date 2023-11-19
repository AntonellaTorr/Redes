#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

#define PORT 5000
#define BUFFER_SIZE 1024
#define MAX_PLAYERS 2

; // el gnador 0 empate 1 gano el 1, 2 gan el 2, -1 todavia no hay res

struct state
{
    int table[3][3];
    int turnPlayer;
    int gameState;
    int winner;
};

struct state st;
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int serverInit()
{
    int sockfd;
    // direccion del socket
    struct sockaddr_in serv_addr;

    // Crea el socket del servidor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error al abrir el socket");
    }

    // Inicializa la estructura de dirección del servidor
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Enlaza el socket al puerto
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error en el enlace");
    }

    // Habilita el socket para aceptar conexiones, con una cola de espera de tamaño 2
    listen(sockfd, 2);
    printf(" server listening");
    return sockfd;
}

int connectPlayer(int serverSocket)
{
    // conecta a una jugador al socket del servidor con el nro pasado por parametro

    int socketPlayer;
    // almacena la información de la dirección del cliente que se conecta al servidor.
    struct sockaddr_in pla_addr;
    int pla_length = sizeof(pla_addr);

    while ((socketPlayer = accept(serverSocket, (struct sockaddr *)&pla_addr, &pla_length)) < 0)
        close(socketPlayer);
    printf(" conecte clientesito nro de socket %d", socketPlayer);
    return socketPlayer;
}

void resetGame()
{
    // Rellenar la matriz con valores
    st.turnPlayer = 1;
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {

            st.table[row][column] = 0;
        }
    }
    st.gameState = 0;
    st.winner = -1;
}
struct infoMessage initGameMsg(int playId)
{
    struct infoMessage msg;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            msg.table[i][j] = st.table[i][j];
        }
    }
    msg.gameState = st.gameState;
    msg.turnPlayer = 1;    // turno del jugador 1
    msg.playerId = playId; // id del jugador
    msg.winner = -1;       // no hay ganador (por ahora)

    return msg;
}

void checkGameState()
{
    if (checkHorizontalLine() || checkVerticalLine() || checkDiagonalLine())
    {
        printf("¡Jugador %d", st.turnPlayer, "ha ganado!\n");
        st.gameState = 0;
        st.winner = st.turnPlayer;
    }
    else if (checkWithDraw())
    {
        printf("El juego termina en empate.\n");
        st.gameState = 0;
        st.winner = 0;
    }
    else
    {
        printf("El juego continua\n");
    }
}

// Función para verificar si el juego termina en empate
int checkWithDraw()
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (st.table[i][j] == 0)
            {
                return 0; // Todavía hay celdas vacías, el juego no es un empate
            }
        }
    }
    return 1; // Todas las celdas están ocupadas, el juego es un empate
}

// Función para verificar si hay una línea horizontal
int checkHorizontalLine()
{
    for (int i = 0; i < 3; ++i)
    {
        int contador = 0;
        for (int j = 0; j < 3; ++j)
        {
            if (st.table[i][j] == st.turnPlayer)
            {
                contador++;
            }
        }
        if (contador == 3)
        {
            return 1; // Hay una línea horizontal
        }
    }
    return 0; // No hay línea horizontal
}

// Función para verificar si hay una línea vertical
int checkVerticalLine()
{
    for (int j = 0; j < 3; ++j)
    {
        int contador = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (st.table[i][j] == st.turnPlayer)
            {
                contador++;
            }
        }
        if (contador == 3)
        {
            return 1; // Hay una línea vertical
        }
    }
    return 0; // No hay línea vertical
}

// Función para verificar si hay una línea diagonal
int checkDiagonalLine()
{
    int contadorPrincipal = 0;
    int contadorSecundaria = 0;

    // Verificar diagonal principal
    for (int i = 0; i < 3; ++i)
    {
        if (st.table[i][i] == st.turnPlayer)
        {
            contadorPrincipal++;
        }
    }

    // Verificar diagonal secundaria
    for (int i = 0; i < 3; ++i)
    {
        if (st.table[i][3 - 1 - i] == st.turnPlayer)
        {
            contadorSecundaria++;
        }
    }

    // Devolver 1 si hay una línea en alguna de las diagonales
    return (contadorPrincipal == 3 || contadorSecundaria == 3);
};

void main()
{

    int players[2];
    printf("en el maiiin");
    int socketServer = serverInit();
    int amountPlayers = 0;
    while (amountPlayers < MAX_PLAYERS)
    {
        players[amountPlayers] = connectPlayer(socketServer);
        amountPlayers++;
    }

    resetGame();

    st.gameState = 1;
    struct infoMessage mes[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        mes[i] = initGameMsg(i + 1);
    }

    printTable(st.table);


    // mando el estado inicial del juego
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        sendMessage(players[i], &mes[i]);
    }
 

    struct move actualMove;
    // mientras el juego no haya finalizado

    while (st.gameState)
    {
        printf("antes del receive move ");
        recv_move_fuser(players[st.turnPlayer - 1], &actualMove);
        // si la casila no esta vacia entonces la jugada es incorrecta
     
    
        printf(" dsp del receive move" );
   

        //si es valido 
        if (st.table[actualMove.row][actualMove.column] == 0)
        {
            printf (" ento en lo de valido");
            // actualizo la matriz
            st.table[actualMove.row][actualMove.column] = st.turnPlayer;
            // verifico el estado del juego luego del movimiento
            checkGameState();

            // mandar msj de succes
            mes[st.turnPlayer - 1].moveSuccess = 1;
            printf ("jugador %d", st.turnPlayer);
            printf("\n");
            printf ("valor de succes %d" , mes[st.turnPlayer - 1].moveSuccess );
            printf("\n");
            //aca estaba el errro nunca mandaba el msg
            printf("aca deberia mandar el mensajee de exito al jugador");
            sendMessage(players[st.turnPlayer - 1], &mes[st.turnPlayer - 1]);
            printf("\n");
            // actualizo el turno
            //error aca solo asignabamos nunca actualizaba el turno 
            if (st.turnPlayer == 2)
            {
                st.turnPlayer = 1;
            }
            else
            {
                st.turnPlayer = 2;
            }
            printf (" ACTUALICE EL TURN %d", st.turnPlayer);

            // le mandas a los jugadores eso
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    mes[0].table[i][j] = st.table[i][j];
                    mes[1].table[i][j] = st.table[i][j];
                }
            }

            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                mes[i].gameState= st.gameState;
                mes[i].turnPlayer= st.turnPlayer;
                mes[i].winner=st.winner; 
                printf ("El estado del juego %d", st.gameState);
                printf("\n");
                printf ("El jugador de turno %d", st.turnPlayer);
                printf("\n");
                //avisa 
                sendMessage(players[i], &mes[i]);
            } 

        }
        else
        {

            mes[st.turnPlayer - 1].moveSuccess = 0;
            //le mando el msj de jugada invalida
            sendMessage(players[st.turnPlayer - 1], &mes[st.turnPlayer - 1]);
        }
    }


    printf("saliooo");
}
