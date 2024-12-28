#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers();

    createUsersTable();
    createResultsTable();

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
}

void MainWindow::createUsersTable()
{
    QSqlDatabase DB_Connection = QSqlDatabase::addDatabase("QSQLITE");
    DB_Connection.setDatabaseName("base.db");

    if(DB_Connection.open())
        qDebug() << "Database is connected";
    else
        qDebug() << "Database is not connected";


    if (DB_Connection.open()) {
        qDebug() << "Database is connected";

        // Создаем таблицу users, если она не существует
        QSqlQuery query;
        query.prepare("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT, admin INTEGER)");
        if (query.exec()) {
            qDebug() << "Table 'users' created successfully or already exists.";

            // Проверка, если таблица пустая, добавляем пользователя
            query.prepare("SELECT COUNT(*) FROM users");
            if (query.exec()) {
                query.next();
                int count = query.value(0).toInt();
                if (count == 0) {
                    // Добавляем администратора, если таблица пуста
                    query.prepare("INSERT INTO users (username, password, admin) VALUES (:username, :password, :admin)");
                    query.bindValue(":username", "admin");
                    query.bindValue(":password", "adm123");  // Используйте хеширование паролей в реальных приложениях
                    query.bindValue(":admin", 1);  // admin = 1
                    if (query.exec()) {
                        qDebug() << "Admin user created successfully.";
                    } else {
                        qDebug() << "Error inserting admin user: " << query.lastError().text();
                    }
                } else {
                    qDebug() << "Table 'users' is not empty. Skipping user creation.";
                }
            } else {
                qDebug() << "Error checking users count: " << query.lastError().text();
            }
        } else {
            qDebug() << "Error creating table 'users': " << query.lastError().text();
        }
        query.finish();
    } else {
        qDebug() << "Database connection failed: " << DB_Connection.lastError().text();
    }
}

void MainWindow::createResultsTable()
{
    QSqlDatabase DB_Connection = QSqlDatabase::addDatabase("QSQLITE");
    DB_Connection.setDatabaseName("base.db");

    if(DB_Connection.open())
        qDebug() << "Database is connected";
    else
        qDebug() << "Database is not connected";


    if (DB_Connection.open()) {
        qDebug() << "Database is connected";

        // Создаем таблицу users, если она не существует
        QSqlQuery query;
        query.prepare("CREATE TABLE IF NOT EXISTS results (id INTEGER PRIMARY KEY, date TEXT, GPA TEXT)");
        if (query.exec()) {
            qDebug() << "Table 'results' created successfully or already exists.";
        } else {
            qDebug() << "Error creating table 'results': " << query.lastError().text();
        }
        query.finish();
    } else {
        qDebug() << "Database connection failed: " << DB_Connection.lastError().text();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginClicked()
{
    QString username = ui->loginEdit->text();
    QString password = ui->passEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->labelError->setText("Введите логин и пароль");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT admin FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {

        int isAdmin = query.value("admin").toInt();

        if (isAdmin == 1) {
            ui->labelError->setText("Добро пожаловать, администратор!");
            qDebug() << "Admin logged in: " << username;

            adminWindow = new AdminWindow(this);
            adminWindow->show();
            hide();
        } else {
            ui->labelError->setText("Добро пожаловать, пользователь!");
            qDebug() << "User logged in: " << username;

            userWindow = new UserWindow(this);
            userWindow->setUserData(username, isAdmin);
            userWindow->exec();
            hide();
        }
    } else {
        ui->labelError->setText("Неправильный логин или пароль");
        qDebug() << "Login failed for user: " << username;
    }
    query.finish();
}
