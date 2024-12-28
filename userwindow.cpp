#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &UserWindow::onCloseClicked);
}

UserWindow::~UserWindow()
{
    delete ui;
}


void UserWindow::setUserData(const QString &username, bool isAdmin)
{
    ui->labelUsername->setText("Имя пользователя: " + username);
    ui->labelAdmin->setText("Права администратора: " + QString(isAdmin ? "Да" : "Нет"));
}

void UserWindow::onCloseClicked()
{
    this->close();
}
