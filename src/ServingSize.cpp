#include "ServingSize.h"

double ServingSize::baseMultiplier() const
{
    return m_baseMultiplier;
}

QString ServingSize::unit() const
{
    return m_unit;
}

double ServingSize::defaultValue() const
{
    return m_defaultValue;
}

double ServingSize::multiplier(double units) const {
    return m_baseMultiplier * units;
}

void ServingSize::setDefaultValue(double newDefaultValue)
{
    m_defaultValue = newDefaultValue;
}

void ServingSize::setUnit(const QString &newUnit)
{
    m_unit = newUnit;
}

void ServingSize::setBaseMultiplier(double newBaseMultiplier)
{
    m_baseMultiplier = newBaseMultiplier;
}

ServingSize::ServingSize(double baseMultiplier, QString unit, double defaultValue) :
    m_baseMultiplier(baseMultiplier),
    m_unit(unit),
    m_defaultValue(defaultValue)
{}
