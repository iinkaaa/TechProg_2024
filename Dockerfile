# Используем базовый образ Ubuntu 20.04
FROM ubuntu:20.04

# Устанавливаем необходимые пакеты, включая Qt и moc
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    qt5-default \
    qtbase5-dev-tools

# Копируем исходный код сервера внутрь контейнера
COPY . .


# Генерируем файлы moc для классов QObject
RUN /usr/lib/x86_64-linux-gnu/qt5/bin/moc -o moc_your_qobject_class.cpp your_qobject_class.h

# Компилируем сгенерированные файлы moc
RUN qmake -o Makefile echoServer.pro && make

# Открываем порт, на котором работает сервер
EXPOSE 8080

# Команда для запуска вашего сервера при запуске контейнера
CMD ["./echoServer"]
