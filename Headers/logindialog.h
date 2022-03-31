#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "databasehelper.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

    DatabaseHelper *databaseHelper;

signals:
    void displayWindow();

private slots:
    void onLoginClicked();
    void onSignUpClicked();

    void onLogin();
    void onError();
};

#endif // LOGINDIALOG_H
