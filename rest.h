#ifndef REST_H
#define REST_H

#include <QNetworkReply>
#include <QObject>
#include <QPointer>

class Rest : public QObject
{
    Q_OBJECT

private:
    Rest(const Rest& root) = delete;
    Rest& operator=(const Rest&) = delete;

protected:
    QString baseUrl;
    QString server;
    QString apiToken;

    Rest(QString url);

    QNetworkAccessManager *get(QUrl url);
    void restGet(QString url, std::function<void (QNetworkReply*)> action);
    static QByteArray convertToUTF8(QNetworkReply* reply);

    void connectReplyToErrors(QNetworkReply *reply);


public:
    void setApiKey(const QString& apiKey);
    void setServer(const QString& server);
signals:
    void replyError1(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError error);
};

#endif // REST_H
