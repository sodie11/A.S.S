#include "userdialog.h"
#include "ui_userdialog.h"

UserDialog::UserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonSave, &QPushButton::clicked, this, &UserDialog::onSaveClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &UserDialog::onCancelClicked);
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::setMode(const QString &mode)
{
    currentMode = mode;
    if (mode == "Add") {
        this->setWindowTitle("Добавить пользователя");
        ui->lineEditUsername->clear();
        ui->lineEditPassword->clear();
        ui->checkBoxAdmin->setChecked(false);
    } else if (mode == "Edit") {
        this->setWindowTitle("Изменить пользователя");
    } else if (mode == "Delete") {
        this->setWindowTitle("Удалить пользователя");
        ui->lineEditUsername->setDisabled(true);
        ui->lineEditPassword->setDisabled(true);
        ui->checkBoxAdmin->setDisabled(true);
    }
}

void UserDialog::setUserData(const QString &username, const QString &password, bool isAdmin)
{
    ui->lineEditUsername->setText(username);
    ui->lineEditPassword->setText(password);
    ui->checkBoxAdmin->setChecked(isAdmin);
}

QString UserDialog::getUsername() const
{
    return ui->lineEditUsername->text();
}

QString UserDialog::getPassword() const
{
    return ui->lineEditPassword->text();
}

bool UserDialog::isAdmin() const
{
    return ui->checkBoxAdmin->isChecked();
}

void UserDialog::onSaveClicked()
{
    if (ui->lineEditUsername->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин!");
        return;
    }

    if (currentMode == "Delete") {
        accept();
        return;
    }

    if (ui->lineEditPassword->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пароль не может быть пустым!");
        return;
    }

    if (ui->lineEditPassword->text().length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать минимум 6 символов!");
        return;
    }

    accept();
}

void UserDialog::onCancelClicked()
{
    reject();
}
