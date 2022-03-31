#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class DatabaseHelper : public QObject
{
    Q_OBJECT
    static DatabaseHelper *databaseHelperInstance;
public:


    static DatabaseHelper* getInstance(QObject *parent = nullptr);

    DatabaseHelper(DatabaseHelper &other) = delete;

    void operator=(const DatabaseHelper &) = delete;


    void loadData();
    void saveData(QJsonDocument &jsonDoc);


    void setAPIKey(const QString &apiKey);
    void signUserUp(const QString &emailAddress, const QString &password);
    void signUserIn(const QString &emailAddress, const QString &password);
    void performPOST(const QString &url, const QJsonDocument &payload);
    void parseResponse(const QByteArray &response);

private:

    explicit DatabaseHelper(QObject *parent = nullptr);

    QNetworkAccessManager *networkManager;
    QNetworkReply *networkReply1;
    QNetworkReply *networkReply2;
    QNetworkReply *postReply;

    QString apiKey;
    QString curEmail;
    QString curPassword;
    QString idToken;

public slots:
    void networkReplyReadyRead();
    void tryToClose();
    void onPost();
signals:
    void draggableData(QJsonDocument);
    void dataUploaded();
    void userSignedIn();
    void errorSigningIn();
};

#endif // DATABASEHELPER_H
