#include <exception>
#include <iostream>
#include <stdlib.h>

#include "SerialPort.h"

SerialPort::SerialPort()
{

}

void SerialPort::connect()
{
    // Tentar abrir arquivo responsável pela porta serial e retornar stream
    try
    {
        file = fopen("/dev/ttyUSB0", "w");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';

        disconnect();
    }
    std::cout << "The connection has been established." << std::endl;
}

void SerialPort::disconnect()
{
    // Fechar stream do arquivo responsável pela porta serial
    fclose(file);
}

void SerialPort::write(const int msg)
{
    // Tentar escrever sinal na stream da porta serial
    try
    {
        fprintf(file, "%d", msg);
        std::cout << "Message: \"" << std::to_string(msg) << "\" has been sent to serial port." << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}