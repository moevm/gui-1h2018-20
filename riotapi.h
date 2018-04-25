#ifndef RIOTAPI_H
#define RIOTAPI_H

#include <QMap>
#include <QNetworkReply>
#include <QObject>
#include <QVector>
#include "infostructs.h"
#include "rest.h"

class RiotApi : public Rest
{ 
    Q_OBJECT

private:
    AccountInfo accountInfo;
    LeagueInfo leagueInfo;
    QVector<MatchInfo*> matchesInfo;
    QMap<QString, QString> idToName;

    RiotApi(QString url);
    RiotApi(const RiotApi& root) = delete;
    RiotApi& operator=(const RiotApi&) = delete;

    void fillChampionsList();

public:

    static RiotApi& Instance() {
        static RiotApi theSingleInstance("api.riotgames.com");
        return theSingleInstance;
    }

    QString getNickname() {
        return accountInfo.summonerName;
    }

    QString getChampionName(QString championId) {
        return idToName[championId];
    }

    void requestSummonerInfo(QString summonerName);
    void requestLeagueInfo(QString summonerId);
    void requestRecentMatches(QString accountId);
    void requestMatchInfo(MatchInfo* match);

    const QString getSummonerId();

    int c = 0;
protected slots:
    void accountInfoFinished(QNetworkReply *reply);
    void leagueInfoFinished(QNetworkReply *reply);
    void recentMatchesFinished(QNetworkReply *reply);

signals:
    void summonerNameUpdated(const QString&);
    void accountIdUpdated(const QString&);
    void summonerIdUpdated(const QString&);
    void summonerLevelUpdated(const QString&);
    void summonerProfileIconIdUpdated(const QString&);
    void summonerLeagueInfoUpdated(LeagueInfo&);
    void recentMatchesUpdated(const QVector<MatchInfo*>&);
    void lastMatchInfoParsed();
};

#endif // RIOTAPI_H
