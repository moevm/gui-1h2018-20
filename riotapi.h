#ifndef RIOTAPI_H
#define RIOTAPI_H

#include <QtQml>
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

class RiotApi : public QObject
{ 
    Q_OBJECT

private:
    QString baseUrl = "https://ru.api.riotgames.com";
    QString riotApi;
    AccountInfo accountInfo;
    LeagueInfo leagueInfo;
    QVector<MatchInfo> matchesInfo;
    QMap<QString, QString> idToName;

    RiotApi();
    RiotApi(const RiotApi& root) = delete;
    RiotApi& operator=(const RiotApi&) = delete;

    QNetworkAccessManager *get(QUrl url);

    void connectReplyToErrors(QNetworkReply *reply);
public:

    static RiotApi& Instance() {
        static RiotApi theSingleInstance;
        return theSingleInstance;
    }

    void requestSummonerInfo(QString summonerName);
    void requestLeagueInfo(QString summonerId);
    void requestRecentMatches(QString accountId);

    const QString getSummonerId();

    void setApiKey(const QString& apiKey);
protected slots:
    void replyFinished(QNetworkReply *reply);
    void accountInfoFinished(QNetworkReply *reply);
    void leagueInfoFinished(QNetworkReply *reply);
    void recentMatchesFinished(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError error);

signals:
    void summonerNameUpdated(const QString&);
    void accountIdUpdated(const QString&);
    void summonerIdUpdated(const QString&);
    void summonerLevelUpdated(const QString&);
    void summonerProfileIconIdUpdated(const QString&);
    void summonerLeagueInfoUpdated(LeagueInfo&);
    void recentMatchesUpdated(const QVector<MatchInfo>&);

    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
};

#endif // RIOTAPI_H
