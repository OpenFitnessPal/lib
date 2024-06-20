#ifndef FOODITEM_H
#define FOODITEM_H

#include <QJsonObject>

#include "NutrientUnion.h"
#include "ServingSize.h"

class FoodItem
{
private:
    QString m_brand;
    QString m_name;

    QString m_id;

    NutrientUnion m_nutrients;

    QList<ServingSize> m_servingSizes{};
public:
    /**
     * @brief FoodItem Construct a food-item from a search JSON.
     * @param obj The JSON object found from a search.
     */
    FoodItem(const QJsonObject &obj);

    // /**
    //  * @brief FoodItem Construct a food-item from an HTML page.
    //  * @param html The HTML from the relevant page.
    //  */
    // FoodItem(const QString &html);

    FoodItem() = default;

    QJsonObject toJson() const;

    QString brand() const;
    QString name() const;

    QString prettyName() const;
    QString siteName() const;

    NutrientUnion nutrients() const;

    QList<ServingSize> servingSizes() const;
    QString id() const;
};

bool operator==(const FoodItem &a, const FoodItem &b);

typedef struct FoodServing {
    FoodItem item;
    ServingSize size;
    double units;

    QJsonObject toJson() const;
    static struct FoodServing fromJson(const QJsonObject &obj);
} FoodServing;

bool operator==(const FoodServing &a, const FoodServing &b);

#endif // FOODITEM_H
