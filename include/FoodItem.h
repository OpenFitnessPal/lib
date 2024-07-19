#ifndef FOODITEM_H
#define FOODITEM_H

#include <QJsonObject>

#include "NutrientUnion.h"
#include "ServingSize.h"

class FoodItem
{
    Q_GADGET

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString brand READ brand WRITE setBrand NOTIFY brandChanged FINAL)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(NutrientUnion nutrients READ nutrients WRITE setNutrients NOTIFY nutrientsChanged FINAL)
private:
    QString m_brand;
    QString m_name;

    QString m_id;

    NutrientUnion m_nutrients;

    QList<ServingSize> m_servingSizes{};
    ServingSize m_defaultServing;

public:
    /**
     * @brief FoodItem Construct a food-item from a search JSON.
     * @param obj The JSON object found from a search.
     */
    FoodItem(const QJsonObject &obj);

    FoodItem() = default;

    QJsonObject toJson() const;

    QString brand() const;
    void setBrand(const QString &newBrand);

    QString name() const;
    void setName(const QString &newName);

    QString prettyName() const;
    QString siteName() const;

    NutrientUnion nutrients() const;
    void setNutrients(const NutrientUnion &newNutrients);

    QList<ServingSize> servingSizes() const;
    ServingSize defaultServing() const;
    int defaultServingIdx() const;

    QString id() const;
    void setId(const QString &newId);
};

bool operator==(const FoodItem &a, const FoodItem &b);
bool operator!=(const FoodItem &a, const FoodItem &b);

Q_DECLARE_METATYPE(FoodItem);

typedef struct FoodServing {
    Q_GADGET

    Q_PROPERTY(FoodItem item MEMBER item)
    Q_PROPERTY(ServingSize size MEMBER size)
    Q_PROPERTY(double units MEMBER units)

public:
    FoodItem item;
    ServingSize size;
    double units;

    QJsonObject toJson() const;
    static struct FoodServing fromJson(const QJsonObject &obj);
} FoodServing;

bool operator==(const FoodServing &a, const FoodServing &b);

Q_DECLARE_METATYPE(FoodServing);

#endif // FOODITEM_H
