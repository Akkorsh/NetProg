#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    const char* server_address = "172.16.40.1";
    const int server_port = 7; // порт echo

// Создаем сокет
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
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

// Устанавливаем соединение с сервером
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при соединении с сервером");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Отправляем сообщение на сервер
    char user_message[100];
    printf("Введите ваше сообщение: ");
    fgets(user_message, 100, stdin);
    if (send(client_socket, user_message, strlen(user_message), 0) == -1) {
        perror("Ошибка при отправке данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Получаем ответ от сервера
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Ошибка при получении данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

// Выводим ответ от сервера
    std::cout << "Ответ от сервера: " << buffer << std::endl;

// Закрываем сокет
    close(client_socket);

    return 0;
}
