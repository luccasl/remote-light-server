#include "Server.h"
#include <exception>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <fcntl.h>
#include "sys/socket.h"
#include "netinet/in.h"

Server::Server(char* address, const uint port)
{
    this->address = address;
    this->port = port;
}

// Responsável pela leitura de requisições
void HandleRequests(int sock, void (*OnRequestReceived)(char*))
{
    // Definir tamanho do buffer de leitura
    const uint len_buffer = 1024;
    // Inicializar buffer de leitura com todos elementos iguais a 0
    char buffer[len_buffer] = {0};

    // Monitorar file descriptor de comunicação do servidor
    fd_set input;
    FD_ZERO(&input);
    FD_SET(sock, &input);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 1000 * 1000;
    int n = select(sock + 1, &input, NULL, NULL, &timeout);
    // Nenhum file descriptor pronto para leitura
    if(n == 0)
    {
        return;
    }

    // Tentar ler socket e gravar informações no buffer
    read(sock, buffer, len_buffer);
    printf("Received: %s\n", buffer);
    // Chamar delegate após ler requisição
    OnRequestReceived(buffer);
}

void Server::Listen()
{
    int opt = 1;
    // Representa um conjunto de configurações de rede do servidor em uma estrutura (eg: endereço, porta, protocolo, etc...)
    struct sockaddr_in address;
    // Guardar tamanho da estrutura
    int len_addr = sizeof(address);
    int new_sock;

    // Tentar criar socket de conexão do servidor, com protocolo IPV4, comunicação TCP e tipo de packet transmitido 0(padrão)
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Socket creation error." << std::endl;
        std::cout << strerror(errno) << std::endl;
        return;
    }
    
    // Configurações do file descriptor do servidor
    // Permitir reuso de endereços locais (eg: 192.168.0.10:80 e 0.0.0.0:80) e permitir a ligação de um número indefinido de sockets
    // à mesma combinação de endereços e portas
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Failed to attach socket to port " << port << std::endl;
        std::cout << strerror(errno) << std::endl;
        return;
    }

    // Protocolo IPV4
    address.sin_family = AF_INET;
    // Ligar socket a todas interfaces de rede disponíveis (cabeada, wifi, etc..)
    address.sin_addr.s_addr = INADDR_ANY;
    // Converter porta para ordem de transferência de bytes de rede(Big-endian) e alimentar estrutura
    address.sin_port = htons(port);

    // Tentar ligar socket ao endereço e porta, no protocolo especificado anteriormente
    if(bind(sock_fd, (struct sockaddr*)&address, len_addr) < 0)
    {
        std::cout << "Failed to bind socket to port " << port << std::endl;
        std::cout << strerror(errno) << std::endl;
        return;
    }

    // Preparar servidor para esperar requisições e definir máximo de requisições consecutivas(30)
    if(listen(sock_fd, 30) == -1)
    {
        std::cout << "Could not start listening on port " << port << std::endl;
        std::cout << strerror(errno) << std::endl;
        return;
    }

    std::cout << "Listening..." << std::endl;
    // Indicar que o servidor está esperando requisições e iniciar loop de leitura
    can_listen = true;
    while(can_listen)
    {
        // Esperar por uma conexão e retornar um novo socket
        if((new_sock = accept(sock_fd, (struct sockaddr*)&address, (socklen_t*)&len_addr)) < 0)
        {
            std::cout << "Could not accept request" << std::endl;
            std::cout << strerror(errno) << std::endl;
            continue;
        }
        // Lidar com requisição em um thread separado
        std::thread req_thread(HandleRequests, new_sock, OnRequestReceived);
        req_thread.join();
    }
}

void Server::Stop()
{
    // Se o servidor não está esperando requisições, retornar execução
    if(!can_listen)
    {
        return;
    }

    // Indicar que servidor não está mais esperando requisições e fechar socket aberto
    can_listen = false;
    close(sock_fd);
}