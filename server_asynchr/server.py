import asyncio
import time
import logging

async def handle_client(reader, writer):
    """
    Обработка подключения клиента
    """
    global logger
    global cid
    cid += 1
    client_addr = writer.get_extra_info('peername')  # Находим адрес клиента
    print("Client" + " ", cid, ": ")
    print(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Connected to ' + str(client_addr))
    logger.info(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Connected to ' + str(client_addr))

    time.sleep(3)  # 3-х секундная задержка

    # Читаем данные от клиента
    data = await reader.read(1024)
    print(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Message from client: ' + str(data))
    logger.info(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Message from client: ' + str(data))

    # Отправка данных
    data_reversed = str(data[::-1]) + " | Server was written by A.V.Kozhevnikov M3O-109B-22"  # реверс сообщения
    writer.write(bytes(data_reversed, encoding='UTF-8'))  # Запись данных в буфер
    await writer.drain()  # Отправка клиенту
    print(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Message to client: ' + data_reversed)
    logger.info(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Message to client: ' + data_reversed)

    # Закрываем соединение
    time.sleep(3)  # 3-х секундная задержка
    writer.close()
    print(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Client has disconnected')
    logger.info(time.strftime("%d-%m-%Y %H:%M:%S: ") + 'Client has disconnected')


async def main():
    """
    Главная функция сервера
    """
    global logger
    # Создаем сокет и запускаем сервер на порту 8888
    server = await asyncio.start_server(handle_client, '127.0.0.1', 8888)
    # asyncio.start_server() создает объект сервера, который слушает входящие подключения на заданном хосте
    # и порту, и запускает цикл событий для обработки этих подключений.

    # logging
    print('Server started in ' + time.strftime("%d-%m-%Y %H:%M:%S"))
    logger.info('Server started in ' + time.strftime("%d-%m-%Y %H:%M:%S"))  # Запись в лог-файл

    # Ожидаем подключения клиентов
    async with server:
        await server.serve_forever()  # Перейдет к след. блоку <=> когда отработает handle_client

# create logger
logger = logging.getLogger("App")  # Создаем логгер с данным именем
logger.setLevel(logging.INFO)  # Уровень логгирования
handler = logging.FileHandler("asynchro_serv.log")  # Создаем обработчик файла
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')  # Создаем форматировщик
handler.setFormatter(formatter)  # Добавим форматировщик к обработчику
logger.addHandler(handler)  # Добавляем обработчик к логгеру
# Запускаем сервер
cid = 0
asyncio.run(main())