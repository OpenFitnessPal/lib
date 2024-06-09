#include "OFPManager.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

// TODO: constants
static const QString BASE_URL = "https://www.myfitnesspal.com";

OFPManager::OFPManager(QObject *parent)
    : QObject{parent}
    , m_manager(new QNetworkAccessManager(this))
{}

void OFPManager::search(const QString &query) const
{
    QString newQuery = query;
    newQuery.replace(' ', '+');

    QNetworkRequest req;
    req.setUrl(QUrl(BASE_URL + "/api/nutrition?query=" + newQuery));

    QNetworkReply *reply = m_manager->get(req);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this] {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        QJsonArray array = doc.object().value("items").toArray();

        QList<FoodItem> items{};

        for (QJsonValueRef ref : array) {
            QJsonObject obj = ref.toObject();
            FoodItem item(obj);
            items.append(item);
        }

        emit searchComplete(items);
    });
}
