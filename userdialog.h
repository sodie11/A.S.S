#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = nullptr);
    ~UserDialog();

    void setMode(const QString &mode);
    void setUserData(const QString &username, const QString &password, bool isAdmin);
    QString getUsername() const;
    QString getPassword() const;
    bool isAdmin() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    Ui::UserDialog *ui;
    QString currentMode;
};

#endif // USERDIALOG_H
