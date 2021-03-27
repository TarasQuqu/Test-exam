#include "lib.h"

int main()
{
    int socketId;

    socketId = socketCreate(SOCK_STREAM);

    portBinding(socketId, PORT);

    listenClient(socketId, 128);

    acceptClients(socketId,newConnection);
}
