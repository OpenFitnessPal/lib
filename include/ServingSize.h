#ifndef SERVINGSIZE_H
#define SERVINGSIZE_H

#include <QString>

#include <QJsonObject>

class ServingSize
{
private:
    double m_baseMultiplier;

    QString m_unit;

    double m_defaultValue;
public:
    ServingSize(double baseMultiplier = 0, QString unit = "", double defaultValue = 0);

    double multiplier(double units) const;

    double baseMultiplier() const;
    void setBaseMultiplier(double newBaseMultiplier);

    QString unit() const;
    QString baseUnit() const;
    void setUnit(const QString &newUnit);

    double defaultValue() const;
    void setDefaultValue(double newDefaultValue);

    static ServingSize fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;
};

bool operator==(const ServingSize &a, const ServingSize &b);

#endif // SERVINGSIZE_H
