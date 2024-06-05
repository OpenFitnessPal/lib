#ifndef OFPMANAGER_H
#define OFPMANAGER_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>

#include "FoodItem.h"

class OFPManager : public QObject
{
    Q_OBJECT

    QNetworkAccessManager *m_manager;
public:
    explicit OFPManager(QObject *parent = nullptr);

public slots:
    void search(const QString &query) const;

signals:
    void searchComplete(const QList<FoodItem> results) const;
};

#endif // OFPMANAGER_H
