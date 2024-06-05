#ifndef FOODITEM_H
#define FOODITEM_H

#include <QJsonObject>

#include "ServingSize.h"

class FoodItem
{
private:
    QString m_brand;
    QString m_name;

    QString m_id;

    double m_fat;
    double m_satFat;
    double m_monoFat;
    double m_polyFat;
    double m_transFat;

    double m_carbs;
    double m_fiber;
    double m_sugar;
    double m_addedSugar;

    double m_protein;
    double m_cholesterol;

    double m_calcium;
    double m_iron;
    double m_sodium;
    double m_potassium;

    double m_vitaminA;
    double m_vitaminC;
    double m_vitaminD;

    QList<ServingSize> m_servingSizes{};
public:
    FoodItem(const QJsonObject &obj);
    FoodItem(const QString &html);

    double calories() const;

    QString brand() const;
    QString name() const;

    QString prettyName() const;
    QString siteName() const;

    double fat() const;
    double satFat() const;
    double monoFat() const;
    double polyFat() const;
    double transFat() const;

    double carbs() const;
    double fiber() const;
    double sugar() const;
    double addedSugar() const;

    double protein() const;
    double cholesterol() const;

    double calcium() const;
    double iron() const;
    double sodium() const;
    double potassium() const;

    double vitaminA() const;
    double vitaminC() const;
    double vitaminD() const;

    QList<ServingSize> servingSizes() const;
};

#endif // FOODITEM_H
