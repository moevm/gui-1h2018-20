#include "cachemanager.h"

#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QPixmap>

CacheManager::CacheManager() : QObject(nullptr)
{
    if (!QDir("imgcache").exists()) {
        QDir().mkdir("imgcache");
    } else {
        //fill set of already cached imgs
        QDirIterator it("imgcache", QDir::Files);
        while (it.hasNext()) {
            it.next();
            imgset.insert(it.fileName());
        }
    }

}

QString CacheManager::getPath(QString fileName)
{
    return "imgcache/" + fileName;
}

bool CacheManager::cached(QString fileName)
{
    return imgset.contains(fileName);
}

void CacheManager::cacheImg(QString name, QPixmap img) {
    img.save(getPath(name), "png");
}
