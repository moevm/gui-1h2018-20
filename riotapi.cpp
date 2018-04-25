#include "riotapi.h"
#include <QFile>
#include <QTextStream>
#include <QUrlQuery>
#include <iostream>
#include <QRegExp>
#include <functional>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


RiotApi::RiotApi(QString url) : Rest(url)
{
    fillChampionsList();
}

void RiotApi::fillChampionsList()
{
    QUrl url = QUrl("http://ddragon.leagueoflegends.com/cdn/8.8.2/data/en_US/champion.json");
    QNetworkAccessManager* nam = get(url);
    connect(nam, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        QJsonObject champions = QJsonDocument::fromJson(convertToUTF8(reply)).object()["data"].toObject();

        for (auto start = champions.begin(); start != champions.end(); start++) {
            QJsonObject champion = start->toObject();
            idToName.insert(champion["key"].toString(), champion["id"].toString());
        }
    });
}

void RiotApi::requestRecentMatches(QString accountId)
{
    restGet("/lol/match/v3/matchlists/by-account/"+accountId+"/recent", [=](QNetworkReply* reply) {recentMatchesFinished(reply);});
}

void RiotApi::requestLeagueInfo(QString summonerId)
{
    restGet("/lol/league/v3/positions/by-summoner/"+summonerId, [=](QNetworkReply* reply) {leagueInfoFinished(reply);});
}

void RiotApi::requestSummonerInfo(QString summonerName)
{
    restGet("/lol/summoner/v3/summoners/by-name/"+summonerName, [=](QNetworkReply* reply) {accountInfoFinished(reply);});
}

const QString RiotApi::getSummonerId() { return accountInfo.summonerId; }

void RiotApi::accountInfoFinished(QNetworkReply *reply)
{
    QJsonObject accountJson = QJsonDocument::fromJson(convertToUTF8(reply)).object();

    accountInfo = AccountInfo(accountJson);

    emit summonerNameUpdated         ("<b>"+accountInfo.summonerName+"</b>");
    emit summonerIdUpdated           (accountInfo.summonerId);
    emit accountIdUpdated            (accountInfo.accountId);
    emit summonerLevelUpdated        (accountInfo.summonerLevel);
    emit summonerProfileIconIdUpdated(accountInfo.summonerProfileIconId);

    requestLeagueInfo(accountInfo.summonerId);
}

void RiotApi::leagueInfoFinished(QNetworkReply *reply)
{
    QJsonArray leaguesArray = QJsonDocument::fromJson(convertToUTF8(reply)).array();

    for (auto start = leaguesArray.begin(); start != leaguesArray.end(); start++) {
        QJsonObject league = start->toObject();
        if (league["queueType"].toString() == "RANKED_SOLO_5x5") {
            leagueInfo = LeagueInfo(league);

            emit summonerLeagueInfoUpdated(leagueInfo);
            break;
        }
    }
}


void RiotApi::requestMatchInfo(MatchInfo* match)
{
    restGet("/lol/match/v3/matches/" + match->gameId, [=](QNetworkReply* reply) {
        QJsonObject matchJson = QJsonDocument::fromJson(convertToUTF8(reply)).object();
        match->duration = (matchJson["gameDuration"].toInt() / 60) + 1;
        match->participantIdentities = matchJson["participantIdentities"].toArray();
        match->participants = matchJson["participants"].toArray();
        if (++c == 15) emit lastMatchInfoParsed();
    });
}

void RiotApi::recentMatchesFinished(QNetworkReply *reply)
{
    matchesInfo.clear();
    connect(this, &RiotApi::lastMatchInfoParsed, [=](){ emit recentMatchesUpdated(matchesInfo);});
    c = 0;
    QJsonArray matchesArray = QJsonDocument::fromJson(convertToUTF8(reply)).object()["matches"].toArray();
    int limit = 15;
    for(QJsonArray::iterator start = matchesArray.begin(); start != matchesArray.end(); start++) {
        QJsonObject matchJson = (*start).toObject();
        MatchInfo* match = new MatchInfo(
           QString::number(matchJson["gameId"].toVariant().toLongLong()),
           idToName[QString::number(matchJson["champion"].toInt())],
           QString::number(matchJson["timestamp"].toVariant().toLongLong() / 1000),
           static_cast<QueueType>(matchJson["queue"].toInt()));
        requestMatchInfo(match);
        matchesInfo.append(match);
        if (--limit == 0) break;
    }
}

