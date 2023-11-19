#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    const char* server_address = "172.16.40.1";
    const int server_port = 13; // порт daytime

// Создаем сокет
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

// Настраиваем адрес сервера
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_address, &(server_addr.sin_addr)) <= 0) {
        perror("Ошибка при настройке адреса сервера");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Отправляем запрос на сервер
    if (sendto(client_socket, nullptr, 0, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при отправке данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Получаем ответ от сервера
    char buffer[1024];
    socklen_t server_addr_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_addr_len);

    if (bytes_received == -1) {
        perror("Ошибка при получении данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Преобразуем полученные данные в строку времени
    time_t server_time = ntohl(*reinterpret_cast<time_t*>(buffer));
    std::cout << "Время на сервере " << server_address << ":" << server_port << ": " << ctime(&server_time);

// Закрываем сокет
    close(client_socket);

    return 0;
}
