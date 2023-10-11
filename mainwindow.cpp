#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<bool> session)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , activeSession(session)
{
    ui->setupUi(this);
    sender = "";
    recepient = "";
    dataBaseHandler.connectToDataBase("mainwindow");
    QMap userList = dataBaseHandler.listOfUsers();
    ui->listWidgetRecepient->addItem("_all");
    QMap<QString, QString>::const_iterator i = userList.constBegin();
    while (i != userList.constEnd()) {
        ui->listWidgetUsers->addItem(i.key());
        ui->listWidgetRecepient->addItem(i.key());
        ++i;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUserInfo()
{
    // User info filling:
    ui->listWidgetInfo->clear();
    QString userName = "Name: \t"
                       + dataBaseHandler.findUserNameByLogin(
                           ui->listWidgetUsers->currentItem()->text());
    ui->listWidgetInfo->addItem(userName);

    QString banStatus;
    if (QString::number(dataBaseHandler.checkForban(ui->listWidgetUsers->currentItem()->text()))
            .toInt()
        == 1)
        banStatus = "Ban status:\t banned";
    else
        banStatus = "Ban status:\t clear";
    ui->listWidgetInfo->addItem(banStatus);
}

void MainWindow::on_listWidgetUsers_itemClicked(QListWidgetItem *item)
{
    //std::cout << ui->listWidgetRecepient->currentItem()->text().toStdString();
    sender = item->text();
    ui->listWidgetMessages->clear();
    ui->listWidgetInfo->clear();
    QList<QString> messagesList;

    if (!sender.isEmpty() && !recepient.isEmpty())
        messagesList = dataBaseHandler.listOfMessages(sender, recepient);

    for (auto msg : messagesList)
        ui->listWidgetMessages->addItem(msg);

    updateUserInfo();
}
void MainWindow::on_pushButtonQuit_clicked()
{
    close();
}

void MainWindow::on_pushButtonDeactivate_clicked()
{
    *activeSession = false;
}

void MainWindow::on_listWidgetRecepient_itemClicked(QListWidgetItem *item)
{
    recepient = item->text();
    ui->listWidgetMessages->clear();
    QList<QString> messagesList;
    if (!sender.isEmpty() && !recepient.isEmpty())
        messagesList = dataBaseHandler.listOfMessages(sender, recepient);

    for (auto msg : messagesList)
        ui->listWidgetMessages->addItem(msg);
}

void MainWindow::on_pushButtonBan_clicked()
{
    dataBaseHandler.banUser(ui->listWidgetUsers->currentItem()->text());
    updateUserInfo();
}

void MainWindow::on_pushButtonUnban_clicked()
{
    dataBaseHandler.unbanUser(ui->listWidgetUsers->currentItem()->text());
    updateUserInfo();
}
