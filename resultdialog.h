#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    ~ResultDialog();

    void setMode(const QString &mode);
    void setResultData(const QString &date, const QString &gpa);
    QString getDate() const;
    QString getGPA() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    Ui::ResultDialog *ui;
    QString currentMode;
};

#endif // RESULTDIALOG_H
