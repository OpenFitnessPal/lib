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
    req.setRawHeader("referer", QByteArray("https://www.myfitnesspal.com/nutrition-facts-calories/") + query.toUtf8());
    req.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/6.7.1 Chrome/118.0.5993.220 Safari/537.36");
    req.setRawHeader("accept", "application/json");

    QNetworkReply *reply = m_manager->get(req);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this] {
        QByteArray data = reply->readAll();
        qDebug() << data;
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
