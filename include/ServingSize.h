#ifndef SERVINGSIZE_H
#define SERVINGSIZE_H

#include <QString>

class ServingSize
{
private:
    double m_baseMultiplier;

    QString m_unit;

    double m_defaultValue;
public:
    ServingSize(double baseMultiplier, QString unit, double defaultValue);

    double multiplier(double units) const;

    double baseMultiplier() const;
    QString unit() const;
    double defaultValue() const;
};

#endif // SERVINGSIZE_H
