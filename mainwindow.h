#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DBHandler.h"

QT_BEGIN_NAMESPACE
class QListWidgetItem;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, std::shared_ptr<bool> session = nullptr);
    ~MainWindow();

private slots:
    void on_listWidgetUsers_itemClicked(QListWidgetItem *item);

    void on_pushButtonQuit_clicked();

    void on_pushButtonDeactivate_clicked();

private:
    Ui::MainWindow *ui;
    Data dataBaseHandler;
    std::shared_ptr<bool> activeSession;
    QString currentuser;
};
#endif // MAINWINDOW_H
