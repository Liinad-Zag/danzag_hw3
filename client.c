#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Установка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Подключение к серверу
    if (connect(clientSocket, reinterpret_cast
	
	const sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to connect to server\n";
        close(clientSocket);
        return 1;
    }

    // Сбор и отправка RRC Connection Setup Request
    std::string setupRequest = "RRC Connection Setup Request: UE IP = 192.168.1.1, Establishment Cause = Emergency";
    send(clientSocket, setupRequest.c_str(), setupRequest.length(), 0);

    // Получение и обработка RRC Connection Setup от сервера
    char buffer[1024] = {0};
    recv(clientSocket, buffer, 1024, 0);
    std::cout << "Received from server: " << buffer << std::endl;

    // Сбор и отправка RRC Connection Setup Complete на сервер
    std::string setupComplete = "RRC Connection Setup Complete: Transaction ID = 123, NAS = Attach Request, PLMNID = XYZ, DedicatedInfoNAS = ABC";
    send(clientSocket, setupComplete.c_str(), setupComplete.length(), 0);

    // Закрытие сокета
    close(clientSocket);

    return 0;
}
