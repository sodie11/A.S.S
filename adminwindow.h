#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void onSwitchPageToResult();
    void onSwitchPageToTest();
    void onApplicationExit();

    void loadUsers();
    void onAddUser();
    void onDeleteUser();
    void onEditUser();

    void loadResults();
    void onAddResult();
    void onDeleteResult();
    void onEditResult();

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
