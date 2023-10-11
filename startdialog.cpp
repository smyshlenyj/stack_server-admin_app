#include "startdialog.h"
#include "ui_startdialog.h"

startdialog::startdialog(QWidget *parent, std::shared_ptr<bool> _signInPassed)
    : QDialog(parent)
    , ui(new Ui::startdialog)
    , signInPassed(_signInPassed)
{
    ui->setupUi(this);
    dataBaseHandler.connectToDataBase("startupwindow");
}

startdialog::~startdialog()
{
    delete ui;
}

void startdialog::on_pushButtonQuit_clicked()
{
    *signInPassed = false;
    close();
    //QCoreApplication::exit();
    //qApp->exit();
}

void startdialog::on_pushButtonSignIn_clicked()
{
    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    if (dataBaseHandler.loginAndPasswordMatch(login, password, true)) {
        *signInPassed = true;
        close();
    } else
        ui->lineEditPassword->clear();
}
