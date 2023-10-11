Клиент (windows, visual studio / linux, cmake) https://github.com/smyshlenyj/chat_gui/
Сервер (linux, QT) https://github.com/smyshlenyj/stack_server-admin_app

Подключение к серверу настраивается в клиенте в Connection.cpp (клиент chat-gui):
std::string serverAddress = "192.168.186.129";

Подключение в БД такое (DBHandler.cpp на сервере stack_server-admin_app):

    sdb = QSqlDatabase::addDatabase("QMYSQL", dbName);
    sdb.setHostName(HOSTNAME);
    sdb.setDatabaseName(DATABASENAME);
    sdb.setUserName(USERNAME);
    sdb.setPassword(PASSWORD);

#define MESSAGE_LENGTH 1024
#define PORT 7777
#define HOSTNAME "localhost"
#define DATABASENAME "chatDB"
#define USERNAME "root"
#define PASSWORD "777"

Сервер создаст таблицы сам, если не найдёт.
В корневой директории репозитория сервера лежат два файла .sql для наполнения базы тестовыми данными.