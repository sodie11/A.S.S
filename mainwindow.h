#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "adminwindow.h"
#include "userwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void createUsersTable();
    void createResultsTable();
private slots:
    void onLoginClicked();

private:
    Ui::MainWindow *ui;
    AdminWindow *adminWindow;
    UserWindow *userWindow;
};
#endif // MAINWINDOW_H
