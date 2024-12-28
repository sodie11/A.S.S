#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonSave, &QPushButton::clicked, this, &ResultDialog::onSaveClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &ResultDialog::onCancelClicked);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setMode(const QString &mode)
{
    currentMode = mode;
    if (mode == "Add") {
        this->setWindowTitle("Добавить результат");
        ui->lineEditDate->clear();
        ui->lineEditGPA->clear();
    } else if (mode == "Edit") {
        this->setWindowTitle("Изменить результат");
    } else if (mode == "Delete") {
        this->setWindowTitle("Удалить результат");
        ui->lineEditDate->setDisabled(true);
        ui->lineEditGPA->setDisabled(true);
    }
}

void ResultDialog::setResultData(const QString &date, const QString &gpa)
{
    ui->lineEditDate->setText(date);
    ui->lineEditGPA->setText(gpa);
}

QString ResultDialog::getDate() const
{
    return ui->lineEditDate->text();
}

QString ResultDialog::getGPA() const
{
    return ui->lineEditGPA->text();
}

void ResultDialog::onSaveClicked()
{
    if (ui->lineEditDate->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите дату!");
        return;
    }

    if (currentMode == "Delete") {
        accept();
        return;
    }

    if (ui->lineEditGPA->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите GPA!");
        return;
    }

    accept();
}

void ResultDialog::onCancelClicked()
{
    reject();
}
