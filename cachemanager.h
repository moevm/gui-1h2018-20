#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QObject>
#include <QSet>

class CacheManager : public QObject
{
    Q_OBJECT
private:
    QSet<QString> imgset;

    CacheManager();
    CacheManager(const CacheManager& root) = delete;
    CacheManager& operator=(const CacheManager&) = delete;
public:
    static CacheManager& Instance() {
        static CacheManager theSingleInstance;
        return theSingleInstance;
    }

    QString getPath(QString fileName);
    bool cached(QString fileName);
    void cacheImg(QString name, QPixmap img);
signals:

public slots:
};

#endif // CACHEMANAGER_H
