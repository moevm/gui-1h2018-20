#ifndef RIOTAPI_H
#define RIOTAPI_H

#include "apibase.h"
#include <QtQml>
#include "jsonrestlistmodel.h"
#include <QVector>

enum QueueType {
    SRRankedSolo = 420, SRBlindPick = 430, SRRankedFlex = 440, ARAM = 450
};

struct AccountInfo {
    QString summonerName;
    QString summonerId;
    QString accountId;
    QString summonerLevel;
    QString summonerProfileIconId;

    AccountInfo() {}
    AccountInfo(QString name, QString sid, QString aid, QString lvl, QString pid) :
        summonerName(name),
        summonerId(sid),
        accountId(aid),
        summonerLevel(lvl),
        summonerProfileIconId(pid){}
};

struct LeagueInfo {
    QString wins;
    QString losses;
    QString rank;
    QString tier;
    QString leaguePoints;

    LeagueInfo() {}
    LeagueInfo(QString w, QString l, QString r, QString t, QString lp) :
        wins(w), losses(l), rank(r), tier(t), leaguePoints(lp){}
};

struct MatchInfo {
    QString champion;
    QString timestamp;
    QueueType queue;

    MatchInfo() {}
    MatchInfo(QString c, QString ts, QueueType q) :
        champion(c), timestamp(ts), queue(q) {}
};

class RiotApi : public APIBase
{ 
    Q_OBJECT

private:
    QString baseUrl = "https://ru.api.riotgames.com";
    AccountInfo accountInfo;
    LeagueInfo leagueInfo;
    QVector<MatchInfo> matchesInfo;

    Q_INVOKABLE explicit RiotApi();
    RiotApi(const RiotApi& root) = delete;
    RiotApi& operator=(const RiotApi&) = delete;

    QNetworkAccessManager *get(QUrl url);
public:

    static RiotApi& Instance() {
        static RiotApi theSingleInstance;
        return theSingleInstance;
    }

    void requestSummonerInfo(QString summonerName);
    void requestLeagueInfo(QString summonerId);
    void requestRecentMatches(QString accountId);

    const QString getSummonerId();

protected slots:
    virtual void replyFinished(QNetworkReply *reply) override;
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
    void recentMatchesUpdated(const QVector<MatchInfo>&);
};

#endif // RIOTAPI_H
