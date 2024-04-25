#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Создание сокета
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Установка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу
    if (bind(serverSocket, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind socket\n";
        close(serverSocket);
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Failed to listen\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Waiting for a connection...\n";

    // Принятие входящего соединения
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, reinterpret_cast<const sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept connection\n";
        close(serverSocket);
        return 1;
    }

    // Получение и обработка RRC Connection Setup Request от клиента
    char buffer[1024] = {0};
    recv(clientSocket, buffer, 1024, 0);
    std::cout << "Received from client: " << buffer << std::endl;

    // Формирование и отправка RRC Connection Setup на клиент
    std::string setupResponse = "RRC Connection Setup: Configuration SRB1 = Default, UE10 = Default, MAC Transaction ID = 456";
    send(clientSocket, setupResponse.c_str(), setupResponse.length(), 0);

    // Получение и обработка RRC Connection Setup Complete от клиента
    memset(buffer, 0, sizeof(buffer)); // Очистка буфера перед получением новых данных
    recv(clientSocket, buffer, 1024, 0);
    std::cout << "Received from client: " << buffer << std::endl;

    // Закрытие сокета
    close(clientSocket);
    close(serverSocket);

    return 0;
}
