#include <QCoreApplication>

#include "OFPManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OFPManager *manager = new OFPManager(&a);

    manager->search("New York Strip");
    manager->connect(manager, &OFPManager::searchComplete, manager, [](QList<FoodItem> items) {
        for (const FoodItem &item : items) {
            qInfo() << QString(item.prettyName() + ":").toStdString().c_str() << item.calories() << "kcal";
        }
    });

    return a.exec();
}
