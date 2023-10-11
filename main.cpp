#include "DBHandler.h"
#include "mainwindow.h"
#include "server.h"
#include "startdialog.h"

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <memory>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Data dataHandlerServer;

    std::shared_ptr<bool> activeSession{std::make_shared<bool>(false)};
    dataHandlerServer.connectToDataBase("background");
    std::thread th1(server, dataHandlerServer, activeSession);
    th1.detach();

    std::shared_ptr<bool> signInPassed{std::make_shared<bool>(false)};
    startdialog *s = new startdialog(nullptr, signInPassed);
    s->setWindowTitle("Stack - past generation messenger admin panel");
    s->setWindowIcon(QIcon(":/ico.png"));
    s->exec();

    if (*signInPassed) {
        MainWindow *w = new MainWindow(nullptr, activeSession);
        w->setWindowTitle("Stack - past generation messenger admin panel");
        w->setWindowIcon(QIcon(":/ico.png"));
        w->show();
    }
    return a.exec();
}
