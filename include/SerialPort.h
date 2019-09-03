#ifndef _SERIAL_PORT_H
#define _SERIAL_PORT_H

#include <stdio.h>

// Responsável pela comunicação entre a porta serial e o servidor
class SerialPort
{
public:
    SerialPort();

    // Conectar à porta serial
    void connect();
    // Fechar comunicação com a porta serial
    void disconnect();

    // Enviar sinais para a porta serial
    void write(const int msg);
    // Ler sinais recebidos pela porta serial
    int* read();

private:
    // Stream do arquivo da porta serial dentro do sistema
    FILE* file;
};

#endif