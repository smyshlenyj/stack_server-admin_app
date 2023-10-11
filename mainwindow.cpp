#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<bool> session)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , activeSession(session)
{
    ui->setupUi(this);
    dataBaseHandler.connectToDataBase("mainwindow");
    QMap userList = dataBaseHandler.listOfUsers();
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

void MainWindow::on_listWidgetUsers_itemClicked(QListWidgetItem *item)
{
    //std::cout << ui->listWidgetRecepient->currentItem()->text().toStdString();

    ui->listWidgetMessages->clear();
    ui->listWidgetInfo->clear();
    auto messagesList = dataBaseHandler.listOfMessages(ui->listWidgetRecepient->currentItem()->text(),
                                                       item->text());

    for (auto msg : messagesList)
        ui->listWidgetMessages->addItem(msg);

    QString userName = "Name: \t" + dataBaseHandler.findUserNameByLogin(item->text());
    ui->listWidgetInfo->addItem(userName);

    QString banStatus;
    if (QString::number(dataBaseHandler.checkForban(item->text())).toInt() == 1)
        banStatus = "Ban status:\t banned";
    else
        banStatus = "Ban status:\t clear";
    ui->listWidgetInfo->addItem(banStatus);
}

void MainWindow::on_pushButtonQuit_clicked()
{
    close();
}

void MainWindow::on_pushButtonDeactivate_clicked()
{
    *activeSession = false;
}
