#include "riotapi.h"
#include <QFile>
#include <QTextStream>
#include <QUrlQuery>
#include <iostream>
#include <QRegExp>
#include <functional>


RiotApi::RiotApi() {}

void RiotApi::requestRecentMatches(QString accountId)
{
    QUrl url = QUrl(baseUrl+"/lol/match/v3/matchlists/by-account/"+accountId+"/recent");

    url.setQuery("api_key=" + riotApi);

    QNetworkAccessManager* nam = get(url);
    connect(nam, &QNetworkAccessManager::finished, this, &RiotApi::recentMatchesFinished);
}

void RiotApi::requestLeagueInfo(QString summonerId)
{
    QUrl url = QUrl(baseUrl+"/lol/league/v3/positions/by-summoner/"+summonerId);

    url.setQuery("api_key=" + riotApi);

    QNetworkAccessManager* nam = get(url);
    connect(nam, &QNetworkAccessManager::finished, this, &RiotApi::leagueInfoFinished);
}

void RiotApi::requestSummonerInfo(QString summonerName)
{
    QUrl url = QUrl(baseUrl+"/lol/summoner/v3/summoners/by-name/"+summonerName);
    url.setQuery("api_key=" + riotApi);

    QNetworkAccessManager* nam = get(url);
    connect(nam, &QNetworkAccessManager::finished, this, &RiotApi::accountInfoFinished);
}

const QString RiotApi::getSummonerId() { return accountInfo.summonerId; }

void RiotApi::setApiKey(const QString &apiKey)
{
    riotApi = apiKey;
}

void RiotApi::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {

    } else {
        qDebug() << reply->error() << reply->errorString();
        emit replyError(reply, reply->error(), reply->errorString());
    }
}

void RiotApi::accountInfoFinished(QNetworkReply *reply)
{
    QByteArray content = reply->readAll();

    QString rawReply = QTextCodec::codecForName("utf8")->toUnicode(content.data());
    QJsonObject itemObject = QJsonDocument::fromJson(rawReply.toUtf8()).object();
    accountInfo = AccountInfo(
        itemObject["name"].toString(),
        QString::number(itemObject["id"].toInt()),
        QString::number(itemObject["accountId"].toInt()),
        QString::number(itemObject["summonerLevel"].toInt()),
        QString::number(itemObject["profileIconId"].toInt())
    );

    emit summonerNameUpdated("<b>"+accountInfo.summonerName+"</b>");
    emit summonerIdUpdated(accountInfo.summonerId);
    emit accountIdUpdated(accountInfo.accountId);
    emit summonerLevelUpdated(accountInfo.summonerLevel);
    emit summonerProfileIconIdUpdated(accountInfo.summonerProfileIconId);

    requestLeagueInfo(accountInfo.summonerId);

}

void RiotApi::leagueInfoFinished(QNetworkReply *reply)
{
    QByteArray content = reply->readAll();

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString rawReply = codec->toUnicode(content.data());
    QJsonArray leaguesArray = QJsonDocument::fromJson(rawReply.toUtf8()).array();

    for(QJsonArray::iterator start = leaguesArray.begin(); start != leaguesArray.end(); start++) {
        QJsonObject league = (*start).toObject();
        if (league["queueType"].toString() == "RANKED_SOLO_5x5") {
            leagueInfo = LeagueInfo(
                QString::number(league["wins"].toInt()),
                QString::number(league["losses"].toInt()),
                league["tier"].toString(),
                league["rank"].toString(),
                QString::number(league["leaguePoints"].toInt())
            );

            emit summonerLeagueInfoUpdated(leagueInfo);
            break;
        }
    }
}

void RiotApi::recentMatchesFinished(QNetworkReply *reply)
{
    QByteArray content = reply->readAll();

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString rawReply = codec->toUnicode(content.data());
    QJsonArray matchesArray = QJsonDocument::fromJson(rawReply.toUtf8()).object()["matches"].toArray();
    matchesInfo.clear();
    for(QJsonArray::iterator start = matchesArray.begin(); start != matchesArray.end(); start++) {
        QJsonObject match = (*start).toObject();
        matchesInfo.append(MatchInfo(
                               QString::number(match["champion"].toInt()),
                               QString::number(match["timestamp"].toVariant().toLongLong() / 1000),
                               static_cast<QueueType>(match["queue"].toInt())
                           ));
    }
    emit recentMatchesUpdated(matchesInfo);
}

QNetworkAccessManager* RiotApi::get(QUrl url)
{
    QNetworkRequest request = QNetworkRequest(url);
    QNetworkAccessManager* nam = new QNetworkAccessManager(this);
    QNetworkReply *reply = nam->get(request);
    connectReplyToErrors(reply);
    return nam;
}


void RiotApi::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "Error" << error;
}

void RiotApi::connectReplyToErrors(QNetworkReply *reply)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
}

