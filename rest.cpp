#include "rest.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>
#include <QString>
#include <QTextCodec>
#include <functional>

Rest::Rest(QString url): baseUrl(url) {}

QByteArray Rest::convertToUTF8(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        return QTextCodec::codecForName("utf8")->toUnicode(reply->readAll().data()).toUtf8();
    } else {
        return QString("").toUtf8();
    }
}

void Rest::setApiKey(const QString &apiKey)
{
    apiToken = apiKey;
}

void Rest::setServer(const QString &server)
{
    if (server == "EUW") this->server = "euw1"; else
    if (server == "EUNE") this->server = "eun1"; else
    if (server == "NA") this->server = "na1"; else
    if (server == "BR") this->server = "br1"; else
    this->server = server;
}

void Rest::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {

    } else {
        qDebug() << reply->error() << reply->errorString();
    }
}

QNetworkAccessManager* Rest::get(QUrl url)
{
    QNetworkRequest request = QNetworkRequest(url);
    QNetworkAccessManager* nam = new QNetworkAccessManager(this);
    QNetworkReply *reply = nam->get(request);
    connectReplyToErrors(reply);
    return nam;
}

void Rest::restGet(QString url, std::function<void (QNetworkReply*)> action)
{
    QUrl fullUrl = QUrl("https://" + server + "." + baseUrl + url);
    fullUrl.setQuery("api_key=" + apiToken);
    QNetworkAccessManager* nam = get(fullUrl);

    connect(nam, &QNetworkAccessManager::finished, action);

}

void Rest::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "Error" << error;
    emit replyErrorSig(error);
}

void Rest::connectReplyToErrors(QNetworkReply *reply)
{
    connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error),
            this, &Rest::replyError);
}
