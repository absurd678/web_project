#pragma comment(lib, "ws2_32.lib") // Динамическая библиотека ядра ОС
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <chrono> 

#pragma warning(disable: 4996) //убирает предупреждение от vs
using namespace std;

static inline char* _cdecl get_time() {
    auto now = chrono::system_clock::now();
    time_t end_time = chrono::system_clock::to_time_t(now);
    return ctime(&end_time);
}

int main()
{
    // ИНИЦИАЛИЗАЦИЯ
    char ServIP[256], ServPort[256]; // IP, порт сервера

    ifstream cfgFile;
    cfgFile.open("client_config.txt");


    ofstream logFile;
    logFile.open("client_log.txt");


    WSAData wsaData;
    int erStat = WSAStartup(MAKEWORD(2, 1), &wsaData); // Запуск сокетов
    if (erStat != 0) {
        logFile << "ERR: can`t make a socket " << get_time() << endl;
        return 0;
    }



    // СЧИТЫВАНИЕ ФАЙЛА КОНФИГУРАЦИИ
    unsigned int port, flag_index = 0, i = 0;
    while (!cfgFile.eof()) {
        cfgFile >> ServIP[i];
        i++;

    }
    ServIP[i - 1] = '\0';

    bool flag = false;
    int j = 0;
    for (int i = 0; i < sizeof(ServIP); i++) {
        if (ServIP[i] == ':') {
            flag = true;
            flag_index = i;
        }
        if (flag && ServIP[i] != ':') {
            ServPort[j] = ServIP[i];
            ServPort[j + 1] = '\0';
            j++;
        }
    }
    ServIP[flag_index] = '\0'; // "...:..." --> "...\0..."
    port = atoi(ServPort);


    SOCKADDR_IN servInfo;
    servInfo.sin_addr.s_addr = inet_addr(ServIP);
    servInfo.sin_port = htons(port);
    servInfo.sin_family = AF_INET;

    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, NULL);
    erStat = connect(ClientSock, (SOCKADDR*)&servInfo, sizeof(servInfo)); // Подключение к серверному сокету
    if (erStat != 0) {
        logFile << "ERR: failed connection " << get_time() << endl;
        return 1;
    }
    else
        cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
        << endl;

    logFile << "INFO: client connected to server " << ServIP << ":" << ServPort << " " << get_time() << endl;
    char data[256] = "Kozhevnikov Artem M3O-109B-22";
    int len = 0;
    for (int i = 0; i <= sizeof(data) - 1; i++) {
        if (data[i] == '\0') {
            len = i;
            break;
        }
    }
    send(ClientSock, data, len, NULL);
    logFile << "INFO: message to server: " << data << " " << get_time() << endl;
    recv(ClientSock, data, sizeof(data), NULL);
    logFile << "INFO: message from server: " << data << " " << get_time() << endl;
    cout << "Server: " << data << endl;

    Sleep(120);
    closesocket(ClientSock);
    cfgFile.close();
    logFile.close();
}
