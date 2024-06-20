#include "OFPManager.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

// TODO: constants
static const QString BASE_URL = "https://search.swurl.xyz/";

OFPManager::OFPManager(QObject *parent)
    : QObject{parent}
    , m_manager(new QNetworkAccessManager(this))
{}

void OFPManager::search(const QString &query) const
{
    QString newQuery = query;
    newQuery.replace(' ', '+');

    QNetworkRequest req;
    req.setUrl(QUrl(BASE_URL + newQuery));

    QNetworkReply *reply = m_manager->get(req);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this] {
        QByteArray data = reply->readAll();
        data.removeLast();

        QJsonParseError e;
        QJsonDocument doc = QJsonDocument::fromJson(data, &e);

        QJsonArray array = doc.array();

        QList<FoodItem> items{};

        for (QJsonValueRef ref : array) {
            QJsonObject obj = ref.toObject();
            FoodItem item(obj);
            items.append(item);
        }

        emit searchComplete(items);
    });

    QObject::connect(reply, &QNetworkReply::errorOccurred, this, [reply, this](QNetworkReply::NetworkError e) {
        qDebug() << e;
    });
}
