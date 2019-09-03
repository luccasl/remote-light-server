// *IMPORTANTE*
// Este programa necessita de uma conexão serial ativa com um Arduino que consiga interpretar
// os sinais especificados.
// O servidor também assume que exista um transmissor de requisições que se adequem às definições
// Este servidor foi desenvolvido com um ambiente Linux(Ubuntu 64 bits) em mente.

#include <iostream>
#include <string.h>

#include "SerialPort.h"
#include "Server.h"

using namespace std;

// Objeto responsável pela comunicação serial
SerialPort* s_port = new SerialPort();
// Objeto responsável pela comunicação de rede
Server* server = new Server();

// Chamado ao receber requisições
void OnRequestReceived(char* msg)
{
    std::cout << "Message: " << msg << std::endl;
    std::cout << "Length: " << sizeof(msg) / sizeof(char) << std::endl;
    // Sinal 1 - encaminhar para a porta serial que a luz deve ser acesa
    if(strcmp(msg, "signal 1") == 0)
    {
        s_port->connect();
        s_port->write(1);
        s_port->disconnect();
    }
    // Sinal -1 - encaminhar para a porta serial que a luz deve ser apagada
    else if (strcmp(msg, "signal -1") == 0)
    {
        s_port->connect();
        s_port->write(0);
        s_port->disconnect();
    }
}

int main()
{
    // Definir procedimento após receber requisição e tentar começar a ler requisições
    try
    {
        server->OnRequestReceived = &OnRequestReceived;
        server->Listen();
    }
    catch(const std::exception& e)
    {
        server->Stop();
        std::cerr << e.what() << '\n';
    }

    return 0;
}