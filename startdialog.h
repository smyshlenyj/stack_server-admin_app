#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include "DBHandler.h"

namespace Ui {
class startdialog;
}

class startdialog : public QDialog
{
    Q_OBJECT

public:
    explicit startdialog(QWidget *parent = nullptr, std::shared_ptr<bool> signInPassed = nullptr);
    ~startdialog();

private slots:
    void on_pushButtonQuit_clicked();

    void on_pushButtonSignIn_clicked();

private:
    Ui::startdialog *ui;
    Data dataBaseHandler;
    std::shared_ptr<bool> signInPassed;
};

#endif // STARTDIALOG_H
