#include "infostructs.h"
#include "riotapi.h"

AccountInfo::AccountInfo() {}

AccountInfo::AccountInfo(QString name, QString sid, QString aid, QString lvl, QString pid) :
    summonerName(name),
    summonerId(sid),
    accountId(aid),
    summonerLevel(lvl),
    summonerProfileIconId(pid){}

AccountInfo::AccountInfo(QJsonObject& accountJson) :
    summonerName                         (accountJson["name"].toString()),
    summonerId           (QString::number(accountJson["id"].toInt())),
    accountId            (QString::number(accountJson["accountId"].toInt())),
    summonerLevel        (QString::number(accountJson["summonerLevel"].toInt())),
    summonerProfileIconId(QString::number(accountJson["profileIconId"].toInt())){}

LeagueInfo::LeagueInfo() {}

LeagueInfo::LeagueInfo(QString w, QString l, QString r, QString t, QString lp) :
    wins(w), losses(l), rank(r), tier(t), leaguePoints(lp){}

LeagueInfo::LeagueInfo(QJsonObject league) :
    wins(QString::number(league["wins"].toInt())),
    losses(QString::number(league["losses"].toInt())),
    rank(league["rank"].toString()),
    tier(league["tier"].toString()),
    leaguePoints(QString::number(league["leaguePoints"].toInt())){}

QMap<int, QList<QString>> MatchInfo::getParticipants() const {
    QMap<int, QList<QString>> result;
    for(int i = 0; i < participantIdentities.size(); i++) {
        QJsonObject identity = participantIdentities[i].toObject();
        result.insert(identity["participantId"].toInt(),
                      QList<QString>() << identity["player"].toObject()["summonerName"].toString());
    }
    for(int i = 0; i < participants.size(); i++) {
        QJsonObject participant = participants[i].toObject();
        result[participant["participantId"].toInt()]
                .append(RiotApi::Instance()
                        .getChampionName(QString::number(participant["championId"].toInt())));

    }
    return result;
}
