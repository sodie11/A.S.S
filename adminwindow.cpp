#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "userdialog.h"
#include "resultdialog.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    ui->tableWidgetUsers->setColumnCount(3);
    ui->tableWidgetUsers->setHorizontalHeaderLabels(QStringList() << "ID" << "Логин" << "Админ");

    ui->tableWidgetResults->setColumnCount(3);
    ui->tableWidgetResults->setHorizontalHeaderLabels(QStringList() << "ID" << "Дата" << "GPA");

    connect(ui->pushButtonResult, &QPushButton::clicked, this, &AdminWindow::onSwitchPageToResult);
    connect(ui->pushButtonTest, &QPushButton::clicked, this, &AdminWindow::onSwitchPageToTest);
    connect(ui->pushButtonExit, &QPushButton::clicked, this, &AdminWindow::onApplicationExit);

    connect(ui->pushButtonAddUser, &QPushButton::clicked, this, &AdminWindow::onAddUser);
    connect(ui->pushButtonEditUser, &QPushButton::clicked, this, &AdminWindow::onEditUser);
    connect(ui->pushButtonDeleteUser, &QPushButton::clicked, this, &AdminWindow::onDeleteUser);

    connect(ui->pushButtonAddResult, &QPushButton::clicked, this, &AdminWindow::onAddResult);
    connect(ui->pushButtonEditResult, &QPushButton::clicked, this, &AdminWindow::onEditResult);
    connect(ui->pushButtonDeleteResult, &QPushButton::clicked, this, &AdminWindow::onDeleteResult);

    loadUsers();
    loadResults();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::onSwitchPageToResult()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminWindow::onSwitchPageToTest()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminWindow::onApplicationExit()
{
    this->close();
}

void AdminWindow::loadUsers()
{
    QSqlQuery query("SELECT id, username, admin FROM users");
    ui->tableWidgetUsers->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidgetUsers->insertRow(row);

        // Установка данных в каждую колонку
        ui->tableWidgetUsers->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidgetUsers->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableWidgetUsers->setItem(row, 2, new QTableWidgetItem(query.value("admin").toInt() == 1 ? "Yes" : "No"));

        ++row;
    }
    query.finish();
}

void AdminWindow::onAddUser()
{
    UserDialog dialog(this);
    dialog.setMode("Add");

    if (dialog.exec() == QDialog::Accepted) {
        QString username = dialog.getUsername();
        QString password = dialog.getPassword();
        bool isAdmin = dialog.isAdmin();

        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password, admin) VALUES (:username, :password, :admin)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":admin", isAdmin ? 1 : 0);

        if (query.exec()) {
            QMessageBox::information(this, "Успешно", "Пользователь добавлен");
            loadUsers();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить пользователя");
        }
        query.finish();
    }
}

void AdminWindow::onDeleteUser()
{
    int row = ui->tableWidgetUsers->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }

    int id = ui->tableWidgetUsers->model()->data(ui->tableWidgetUsers->model()->index(row, 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this, "Успешно", "Пользователь удалён");
        loadUsers();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить пользователя");
    }
    query.finish();
}

void AdminWindow::onEditUser()
{
    int currentRow = ui->tableWidgetUsers->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для редактирования!");
        return;
    }

    int id = ui->tableWidgetUsers->model()->data(ui->tableWidgetUsers->model()->index(currentRow, 0)).toInt();
    QString username = ui->tableWidgetUsers->model()->data(ui->tableWidgetUsers->model()->index(currentRow, 1)).toString();
    bool isAdmin = ui->tableWidgetUsers->model()->data(ui->tableWidgetUsers->model()->index(currentRow, 2)).toInt() == 1;//2

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE id = :id");
    query.bindValue(":id", id);

    QString oldPassword;
    if (query.exec() && query.next()) {
        oldPassword = query.value(0).toString();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить пароль пользователя.");
        return;
    }
    query.finish();

    UserDialog dialog(this);
    dialog.setMode("Edit");
    dialog.setUserData(username, oldPassword, isAdmin);

    if (dialog.exec() == QDialog::Accepted) {
        QString newUsername = dialog.getUsername();
        QString newPassword = dialog.getPassword();
        bool newIsAdmin = dialog.isAdmin();

        QSqlQuery query;
        query.prepare("UPDATE users SET username = :username, password = :password, admin = :admin WHERE id = :id");
        query.bindValue(":username", newUsername);
        query.bindValue(":password", newPassword.isEmpty() ? "password" : newPassword);
        query.bindValue(":admin", newIsAdmin ? 1 : 0);
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Успешно", "Данные пользователя обновлены");
            loadUsers();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные пользователя");
        }
        query.finish();
    }
}

void AdminWindow::loadResults()
{
    QSqlQuery query("SELECT id, date, GPA FROM results");
    ui->tableWidgetResults->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidgetResults->insertRow(row);

        // Установка данных в каждую колонку
        ui->tableWidgetResults->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidgetResults->setItem(row, 1, new QTableWidgetItem(query.value("date").toString()));
        ui->tableWidgetResults->setItem(row, 2, new QTableWidgetItem(query.value("GPA").toString()));

        ++row;
    }
}

void AdminWindow::onAddResult()
{
    ResultDialog dialog(this);
    dialog.setMode("Add");

    if (dialog.exec() == QDialog::Accepted) {
        QString date = dialog.getDate();
        QString gpa = dialog.getGPA();

        QSqlQuery query;
        query.prepare("INSERT INTO results (date, GPA) VALUES (:date, :gpa)");
        query.bindValue(":date", date);
        query.bindValue(":gpa", gpa);

        if (query.exec()) {
            QMessageBox::information(this, "Успешно", "Результат добавлен");
            loadResults();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить результат");
        }
        query.finish();
    }
}

void AdminWindow::onDeleteResult()
{
    int row = ui->tableWidgetResults->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите результат");
        return;
    }

    int id = ui->tableWidgetResults->model()->data(ui->tableWidgetResults->model()->index(row, 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM results WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this, "Успешно", "Результат удалён");
        loadResults();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить результат");
    }
    query.finish();
}

void AdminWindow::onEditResult()
{
    int currentRow = ui->tableWidgetResults->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите результат для редактирования!");
        return;
    }

    int id = ui->tableWidgetResults->model()->data(ui->tableWidgetResults->model()->index(currentRow, 0)).toInt();
    QString date = ui->tableWidgetResults->model()->data(ui->tableWidgetResults->model()->index(currentRow, 1)).toString();

    QSqlQuery query;
    query.prepare("SELECT GPA FROM results WHERE id = :id");
    query.bindValue(":id", id);

    QString oldGPA;
    if (query.exec() && query.next()) {
        oldGPA = query.value(1).toString();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить GPA результата.");
        return;
    }
    query.finish();
    ResultDialog dialog(this);
    dialog.setMode("Edit");
    dialog.setResultData(date, oldGPA);

    if (dialog.exec() == QDialog::Accepted) {
        QString newDate = dialog.getDate();
        QString newGPA = dialog.getGPA();

        QSqlQuery query;
        query.prepare("UPDATE results SET date = :date, GPA = :gpa WHERE id = :id");
        query.bindValue(":date", newDate);
        query.bindValue(":gpa", newGPA.isEmpty() ? "GPA" : newGPA);
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Успешно", "Данные результата обновлены");
            loadResults();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные результата");
        }
        query.finish();
    }
}

