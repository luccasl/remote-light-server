#ifndef _SERVER_H
#define _SERVER_H

#include <stdlib.h>

class Server
{
public:
    // Especificar endereço do servidor e porta
    Server(char* address = "127.0.0.1", const uint port = 4000);

    // Começar a receber requisições
    void Listen();
    // Parar de receber requisições
    void Stop();

    // Evento disparado ao receber uma requisição
    void (*OnRequestReceived)(char*);

private:
    // Endereço do servidor
    char* address;
    // Porta do servidor
    uint port;

    int sock_fd;

    // Indica se o servidor deve receber requisições
    bool can_listen = false;
};

#endif