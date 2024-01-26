![chat_client](https://github.com/smyshlenyj/stack_server_admin_app/assets/103288354/7c966d2b-23eb-416b-8484-041083298368)

![image](https://github.com/smyshlenyj/stack_server_admin_app/assets/103288354/29298f49-0fa5-41cf-8ab4-91bacfe92347)



Клиент (windows, visual studio / linux, cmake) https://github.com/smyshlenyj/chat_gui/

Сервер (linux, QT) https://github.com/smyshlenyj/stack_server-admin_app

Подключение к серверу настраивается в клиенте в Connection.cpp (клиент chat-gui):

std::string serverAddress = "192.168.186.129";

Подключение в БД такое (DBHandler.cpp на сервере stack_server-admin_app):

```
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
```

Сервер создаст таблицы сам, если не найдёт.
В корневой директории репозитория сервера лежат два файла .sql для наполнения базы тестовыми данными.
