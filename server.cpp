#include "server.h"

sockaddr_in serveraddress, client;
socklen_t length;
int sockert_file_descriptor, connection, bind_status, connection_status;
char message[MESSAGE_LENGTH];

void server(Data dataBaseHandler, std::shared_ptr<bool> activeSession)
{
    dataBaseHandler.connectToDataBase("server");
    dataBaseHandler.createDataTable();
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        std::cout << "Socket creation failed.!" << std::endl;
        //    exit(1);
    }

    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor,
                       (struct sockaddr *) &serveraddress,
                       sizeof(serveraddress));
    if (bind_status == -1) {
        std::cout << "Socket binding failed.!" << std::endl;
        // exit(1);
    }

    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections.!" << std::endl;
        // exit(1);
    } else {
        std::cout << "Server is listening for new connection: " << std::endl;
    }
    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr *) &client, &length);
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client.!" << std::endl;
        // exit(1);
    } else
        std::cout << "Communication established" << std::endl;
    dataBaseHandler.createDataTable();
    *activeSession = true;

    // Communication Establishment
    while (activeSession) {
        bzero(message, MESSAGE_LENGTH);
        size_t bytesReadRaw = read(connection, message, MESSAGE_LENGTH);
        if (bytesReadRaw > 0)
            std::cout << "Data received from client: " << message << std::endl;

        std::vector<std::string> array;
        std::stringstream ss(message);
        std::string tmp;
        //size_t i = 0; // iterator for while
        while (std::getline(ss, tmp, '\t'))
            array.push_back(tmp);

        for (size_t i = 0; i < array.size(); ++i) {
            std::cout << array[i] << std::endl;
        }

        if (array.size() > 0 && array[0] == "signIn") {
            if (array.size() == 3
                && dataBaseHandler.loginAndPasswordMatch(QString::fromStdString(array[1]),
                                                         QString::fromStdString(array[2]),
                                                         false)) {
                std::cout << "Login&password are correct" << std::endl;
                write(connection, "true", 5);
            } else {
                std::cout << array.size() << std::endl;
                std::cout << dataBaseHandler.loginAndPasswordMatch(QString::fromStdString(array[1]),
                                                                   QString::fromStdString(array[2]),
                                                                   false)
                          << std::endl;
                std::cout << "Login and/or password is incorrect" << std::endl;
                write(connection, "false", 6);
            }
        }

        if (array.size() > 0 && array[0] == "getUserName") {
            if (array.size() == 2) {
                char reply[MESSAGE_LENGTH];
                bzero(reply, MESSAGE_LENGTH);
                strcpy(reply,
                       dataBaseHandler.findUserNameByLogin(QString::fromStdString(array[1]))
                           .toStdString()
                           .c_str());
                std::cout << "username: " << reply << std::endl;
                write(connection, reply, MESSAGE_LENGTH);
            } else {
                std::cout << "username not found" << std::endl;
                write(connection, "", 1);
            }
        }

        if (array.size() > 0 && array[0] == "getUsers") {
            QMap<QString, QString> listOfUsers = dataBaseHandler.listOfUsers();
            std::string usersQuantity = std::to_string(listOfUsers.size());
            write(connection, usersQuantity.c_str(), MESSAGE_LENGTH);
            std::cout << "getUsers: " << usersQuantity << std::endl;

            for (auto iter{listOfUsers.begin()}; iter != listOfUsers.end(); iter++) {
                bzero(message, MESSAGE_LENGTH);
                QString user = iter.key() + '\t' + iter.value();
                strcpy(message, user.toStdString().c_str());
                write(connection, message, MESSAGE_LENGTH);
                std::cout << "getUsers:SentUser: " << message << std::endl;
            }
        }

        if (array.size() > 0 && array[0] == "getChat") {
            QList<QString> messages = dataBaseHandler
                                          .listOfMessages(QString::fromStdString(array[1]),
                                                          QString::fromStdString(array[2]));
            std::string messagesQuantity = std::to_string(messages.size());
            ssize_t bytesSent = write(connection, messagesQuantity.c_str(), MESSAGE_LENGTH);
            std::cout << "getChat:bytesSent: " << bytesSent
                      << " messagesQuantity: " << messagesQuantity << std::endl;

            for (auto it = messages.cbegin(); it != messages.cend(); it++) {
                char messageFromServer[MESSAGE_LENGTH];
                bzero(messageFromServer, MESSAGE_LENGTH);
                strcpy(messageFromServer, (*it).toStdString().c_str());
                ssize_t bytesSent = write(connection, messageFromServer, MESSAGE_LENGTH);
                std::cout << "getChat:bytesSent: " << bytesSent << std::endl;
                std::cout << "getChat:sendMessage: " << messageFromServer << std::endl;
            }
        }

        if (array.size() > 0 && array[0] == "sendMessage") {
            bool res = dataBaseHandler.receiveMessage(QString::fromStdString(array[1]),
                                                      QString::fromStdString(array[2]),
                                                      QString::fromStdString(array[3]));
            std::string replyFromClient = array[0] + " " + array[1] + " " + array[2] + " "
                                          + array[3];
            std::cout << "inboundMessage: " << replyFromClient << " received status: " << res
                      << std::endl;
        }

        if (array.size() > 0 && array[0] == "uniqueLogin") {
            if (array.size() == 2 && dataBaseHandler.uniqueLogin(QString::fromStdString(array[1]))) {
                std::cout << "TRUE" << std::endl;
                write(connection, "true", 5);
            } else {
                std::cout << "array.size() " << array.size() << std::endl;
                std::cout << "dataBaseHandler.uniqueLogin(QString::fromStdString(array[1]) "
                          << dataBaseHandler.uniqueLogin(QString::fromStdString(array[1]))
                          << std::endl;
                std::cout << "FALSE" << std::endl;
                write(connection, "false", 6);
            }
        }

        if (array.size() > 0 && array[0] == "addUser") {
            dataBaseHandler.addUser(QString::fromStdString(array[1]),
                                    QString::fromStdString(array[2]),
                                    QString::fromStdString(array[3]));
            std::string replyFromClient = array[1] + array[2] + array[3];
            std::cout << "addUser:new user: " << replyFromClient << std::endl;
        }

        if (array.size() > 0 && array[0] == "TerminateSession") {
            std::cout << "Client Exited." << std::endl;
            std::cout << "Server is Exiting..!" << std::endl;
            *activeSession = false;
            break;
        }

        bzero(message, MESSAGE_LENGTH);
        if (*activeSession == false) {
            close(sockert_file_descriptor);
            dataBaseHandler.closeData();
        }
    }

    // closing mysql connection, closing socket, terminating connection
    dataBaseHandler.closeData();
    close(sockert_file_descriptor);
}
