#include "logindialog.h"
#include "ui_logindialog.h"

#include <QPalette>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QPalette palette = ui->pswdLbl->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    ui->pswdLbl->setPalette(palette);

    databaseHelper = DatabaseHelper::getInstance();
    connect(ui->signUpButton, &QPushButton::clicked, this, &LoginDialog::onSignUpClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);

    connect(databaseHelper, &DatabaseHelper::userSignedIn, this, &LoginDialog::onLogin);
    connect(databaseHelper, &DatabaseHelper::errorSigningIn, this, &LoginDialog::onError);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::onLoginClicked()
{
   QString password = ui->pswdLbl->text();
   QString email = ui->userLbl->text();
   databaseHelper->signUserIn(email, password);
}

void LoginDialog::onSignUpClicked()
{
    QString password = ui->pswdLbl->text();
    QString email = ui->userLbl->text();
    databaseHelper->signUserUp(email, password);
}

void LoginDialog::onLogin()
{
    emit displayWindow();
    this->done(1);
}

void LoginDialog::onError()
{
    ui->errorLabel->setText("Error! Please check email and password.");
}
