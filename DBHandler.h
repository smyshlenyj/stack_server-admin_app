#pragma once
#include<mysql/mysql.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QList>
#include <SHA256.h>
#include <mutex>

#define MESSAGE_LENGTH 1024
#define PORT 7777
#define HOSTNAME "localhost"
#define DATABASENAME "chatDB"
#define USERNAME "root"
#define PASSWORD "777"

class Data
{
    QSqlDatabase sdb;

public:
    Data();

    void connectToDataBase(const QString&);
    void closeData();
    void createDataTable();

    bool loginAndPasswordMatch(QString const&, QString const&, bool);
    QString findUserNameByLogin(QString const&);
    QMap<QString, QString> listOfUsers();
    QList<QString> listOfMessages(QString const&, QString const&);
    bool receiveMessage(QString const&, QString const&, QString const&);
    bool uniqueLogin(QString const&);
    bool addUser(QString const&, QString const&, QString const&);
    int checkForban(QString const &);
};

QString hashPassword(const std::string&);
