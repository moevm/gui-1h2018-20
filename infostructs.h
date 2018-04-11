#include <QJsonArray>
#include <QJsonObject>

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
    AccountInfo(QJsonObject& accountJson) :
        summonerName                         (accountJson["name"].toString()),
        summonerId           (QString::number(accountJson["id"].toInt())),
        accountId            (QString::number(accountJson["accountId"].toInt())),
        summonerLevel        (QString::number(accountJson["summonerLevel"].toInt())),
        summonerProfileIconId(QString::number(accountJson["profileIconId"].toInt())){}
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
    LeagueInfo(QJsonObject league) :
        wins(QString::number(league["wins"].toInt())),
        losses(QString::number(league["losses"].toInt())),
        rank(league["rank"].toString()),
        tier(league["tier"].toString()),
        leaguePoints(QString::number(league["leaguePoints"].toInt())){}
};

struct MatchInfo {
    QString gameId;
    QString champion;
    QString timestamp;
    int duration = 0;
    QJsonArray participantIdentities;
    QJsonArray participants;
    QJsonArray teams;
    QueueType queue;

    MatchInfo() {}
    MatchInfo(QString id, QString c, QString ts, QueueType q) :
        gameId(id), champion(c), timestamp(ts), queue(q) {}

    int playerId(QString nickname) const {
        for(int i = 0; i < participantIdentities.size(); i++) {
            if (QString::compare(participantIdentities[i].toObject()["player"].toObject()["summonerName"].toString(), nickname, Qt::CaseInsensitive)) {
                return i;
            };
        }
        return -1;
    }

    QJsonObject getStats(QString nickname) const {
        return participants[playerId(nickname)].toObject()["stats"].toObject();
    }

    int getKills(QString nickname) const {
        return getStats(nickname)["kills"].toInt();
    }

    int getDeaths(QString nickname) const {
        return getStats(nickname)["deaths"].toInt();
    }

    int getAssists(QString nickname) const {
        return getStats(nickname)["assists"].toInt();
    }

    int getMinions(QString nickname) const {
        return getStats(nickname)["totalMinionsKilled"].toInt();
    }

    int getGold(QString nickname) const {
        return getStats(nickname)["goldEarned"].toInt();
    }

    bool isWin(QString nickname) const {
        return getStats(nickname)["win"].toBool();
    }
};