#include "Headers/databasehelper.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DatabaseHelper* DatabaseHelper::databaseHelperInstance{nullptr};
DatabaseHelper *DatabaseHelper::getInstance(QObject *parent)
{
    if(databaseHelperInstance == nullptr)
    {
        databaseHelperInstance = new DatabaseHelper(parent);
    }

    return databaseHelperInstance;
}

DatabaseHelper::DatabaseHelper(QObject *parent) : QObject(parent), apiKey(QString())
{
    networkManager = new QNetworkAccessManager(this);  
}



void DatabaseHelper::loadData()
{
    QString savedEmail(""), savedPassword("");
    for(int i = 0; i < curEmail.size(); i++)
    {
        if(curEmail[i].isLetterOrNumber())
            savedEmail+=curEmail[i];
    }
    for(int i = 0; i < curPassword.size(); i++)
    {
        if(curPassword[i].isLetterOrNumber())
            savedPassword+=curPassword[i];
    }

    networkReply1 = networkManager->get(QNetworkRequest(QUrl("https://personal-organizer-d73b1-default-rtdb.asia-southeast1.firebasedatabase.app/" + savedEmail + savedPassword + ".json?auth=" + idToken)));
    connect(networkReply1, &QNetworkReply::readChannelFinished, this, &DatabaseHelper::networkReplyReadyRead);
}

void DatabaseHelper::saveData(QJsonDocument &jsonDoc)
{
    QString savedEmail(""), savedPassword("");
    for(int i = 0; i < curEmail.size(); i++)
    {
        if(curEmail[i].isLetterOrNumber())
            savedEmail+=curEmail[i];
    }
    for(int i = 0; i < curPassword.size(); i++)
    {
        if(curPassword[i].isLetterOrNumber())
            savedPassword+=curPassword[i];
    }
    QNetworkRequest data(QUrl("https://personal-organizer-d73b1-default-rtdb.asia-southeast1.firebasedatabase.app/" + savedEmail + savedPassword + ".json?auth=" + idToken));
    data.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    networkReply2 = networkManager->put(data, jsonDoc.toJson());
    connect(networkReply2, &QNetworkReply::readyRead, this, &DatabaseHelper::tryToClose);
}


void DatabaseHelper::setAPIKey(const QString &apiKey)
{
    this->apiKey = apiKey;
}

void DatabaseHelper::signUserUp(const QString &emailAddress, const QString &password)
{
    QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + apiKey;

    curEmail = emailAddress;
    curPassword = password;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPOST( signUpEndpoint, jsonPayload );
}

void DatabaseHelper::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey;

    curEmail = emailAddress;
    curPassword = password;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );

    performPOST( signInEndpoint, jsonPayload );
}

void DatabaseHelper::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest postRequest( (QUrl( url )) );
    postRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
    postReply = networkManager->post( postRequest, payload.toJson());
    connect( postReply, &QNetworkReply::readyRead, this, &DatabaseHelper::onPost);
}

void DatabaseHelper::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
    if ( jsonDocument.object().contains("error") )
    {
        qDebug() << "Error occured!" << response;
        emit errorSigningIn();
    }
    else if ( jsonDocument.object().contains("kind"))
    {
        QString newIdToken = jsonDocument.object().value("idToken").toString();
        //qDebug()I<< "Obtained user ID Token: " << idToken;
        qDebug() << "User signed in successfully!";
        idToken = newIdToken;
        emit userSignedIn();
    }
    else
        qDebug() << "The response was: " << response;
}

void DatabaseHelper::networkReplyReadyRead()
{

    QByteArray data;
    data = networkReply1->readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(data));
    emit draggableData(loadDoc);
}

void DatabaseHelper::tryToClose()
{
    emit dataUploaded();
}

void DatabaseHelper::onPost()
{
    QByteArray response = postReply->readAll();
    parseResponse(response);
}
