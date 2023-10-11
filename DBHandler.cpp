#include "DBHandler.h"

Data::Data() {}

void Data::connectToDataBase(const QString &dbName)
{
    sdb = QSqlDatabase::addDatabase("QMYSQL", dbName);
    sdb.setHostName(HOSTNAME);
    sdb.setDatabaseName(DATABASENAME);
    sdb.setUserName(USERNAME);
    sdb.setPassword(PASSWORD);

    if (!sdb.open()) {
        qDebug() << "Cannot open database:" << sdb.lastError();
        exit(1);
    }
    qDebug() << "Database access granted";
}

void Data::closeData()
{
    sdb.close();
}

void Data::createDataTable()
{
    QSqlQuery query(sdb);
    const QString queryTextUsers
        = "CREATE TABLE IF NOT EXISTS users (login varchar(100) not null,"
          "name varchar(100), hashedPassword varchar(100), banned tinyint(1) DEFAULT 0)";

    query.exec(queryTextUsers);
    const QString queryTextMessages
        = "CREATE TABLE IF NOT EXISTS messages (sender varchar(100) not null, recepient "
          "varchar(100), message varchar(500))";
}

QString hashPassword(const std::string &_password)
{
    SHA256 sha;
    sha.update(_password);
    uint8_t* digest = sha.digest();
    std::string result = SHA256::toString(digest);
    delete[] digest;
    return QString::fromStdString(result);
}

bool Data::loginAndPasswordMatch(QString const &login,
                                 QString const &password,
                                 bool passwordNeedsHashing)
{
    // QSqlDatabase db = QSqlDatabase::database("login");

    QString _password = password;
    if (passwordNeedsHashing)
        _password = hashPassword(password.toStdString());

    const QString queryText = "SELECT login, hashedPassword FROM users WHERE login = '" + login
                              + "' AND hashedPassword = '" + _password + "'";
    QSqlQuery query(sdb);

    if (query.exec(queryText) && query.size() == 1)
        return true;
    else
        return false;
}

QString Data::findUserNameByLogin(QString const& login)
{
    QSqlQuery query(sdb);
    const QString queryText = "SELECT name FROM users WHERE login = '" + login + "' LIMIT 1";

    if (query.exec(queryText))
    {
        query.next();
        return query.value(0).toString();
    }
}

QMap<QString, QString> Data::listOfUsers()
{
    QSqlQuery query(sdb);
    const QString queryText = "SELECT login, name FROM users";
    QMap<QString, QString> userEntries;
    if (query.exec(queryText))
        while (query.next())
            userEntries.insert(query.value(0).toString(), query.value(1).toString());

    return userEntries;
}

QList<QString> Data::listOfMessages(QString const& sender, QString const& recepient)
{
    if (recepient == "_all")
    {
        QSqlQuery query(sdb);
        const QString queryText = "SELECT sender, message FROM messages WHERE recepient = '" + recepient + "'";
        QList<QString> messages;
        if (query.exec(queryText))
        {
            while (query.next())
            {
                QString temp = "Public chat\tFrom: ";
                temp += query.value(0).toString();
                temp += "\tMessage: ";
                temp += query.value(1).toString();
                messages.push_back(temp);
            }
        }
        return messages;
    }

    else
    {
        QSqlQuery query(sdb);
        const QString queryText = "SELECT sender, recepient, message FROM messages WHERE (sender = '" + sender + "' AND recepient = '" + recepient + "') OR (sender = '" + recepient + "' AND recepient = '" + sender + "')";
        QList<QString> messages;
        if (query.exec(queryText))
        {
            while (query.next())
            {
                QString temp = "From: ";
                temp += query.value(0).toString();
                temp += "\tTo: ";
                temp += query.value(1).toString();
                temp += "\tMessage: ";
                temp += query.value(2).toString();
                messages.push_back(temp);
            }
        }
        return messages;
    }
}

bool Data::receiveMessage(QString const &sender, QString const &recepient, QString const &message)
{
    QSqlQuery query(sdb);
    const QString queryText = "INSERT INTO messages (sender, recepient, message) VALUES ('" + sender
                              + "', '" + recepient + "', '" + message + "')";
    if (query.exec(queryText))
        return true;
    else
        return false;
}

bool Data::uniqueLogin(QString const& login)
{
    QSqlQuery query(sdb);
    const QString queryText = "SELECT login FROM users WHERE login = '" + login + "'";

    if (query.exec(queryText))
    {
        query.next();

        if (query.size() == 0)
            return true;
        else
            return false;
    }
}

bool Data::addUser(QString const &login, QString const &username, QString const &hashedPassword)
{
    QSqlQuery query(sdb);
    const QString queryText = "INSERT INTO users (login, name, hashedPassword) VALUES ('" + login
                              + "', '" + username + "', '" + hashedPassword + "')";
    if (query.exec(queryText))
        return true;
    else
        return false;
}

int Data::checkForban(QString const &login)
{
    QSqlQuery query(sdb);
    const QString queryText = "SELECT banned FROM users WHERE login = '" + login + "'";
    if (query.exec(queryText)) {
        query.next();
        return query.value(0).toInt();
    } else
        return false;
}

void Data::banUser(QString const &user)
{
    QSqlQuery query(sdb);
    const QString queryText = "UPDATE users SET banned = 1 WHERE login = '" + user + "'";
    query.exec(queryText);
}

void Data::unbanUser(QString const &user)
{
    QSqlQuery query(sdb);
    const QString queryText = "UPDATE users SET banned = 0 WHERE login = '" + user + "'";
    query.exec(queryText);
}
