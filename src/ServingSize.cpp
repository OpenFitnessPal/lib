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

ServingSize::ServingSize(double baseMultiplier, QString unit, double defaultValue) :
    m_baseMultiplier(baseMultiplier),
    m_unit(unit),
    m_defaultValue(defaultValue)
{}
