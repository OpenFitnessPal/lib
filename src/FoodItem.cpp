#include "FoodItem.h"

#include <QJsonArray>
#include <QJsonDocument>

double FoodItem::calories() const
{
    return 4 * (m_carbs - m_fiber) + 4 * m_protein + 9 * m_fat;
}

QString FoodItem::brand() const
{
    return m_brand;
}

QString FoodItem::name() const
{
    return m_name;
}

QString FoodItem::prettyName() const
{
    return m_brand + " " + m_name;
}

QString FoodItem::siteName() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QRegExp regex("[^a-z0-9\\-]");
    static QRegExp multiSpace("[-]{2,4}");
#else
    static QRegularExpression regex("[^a-z0-9\\-]");
    static QRegularExpression multiSpace("[-]{2,4}");
#endif
    return m_name.toLower().replace(" ", "-").replace(regex, "").replace(multiSpace, "-") + "-" + (m_id);
}

double FoodItem::fat() const
{
    return m_fat;
}

double FoodItem::satFat() const
{
    return m_satFat;
}

double FoodItem::monoFat() const
{
    return m_monoFat;
}

double FoodItem::polyFat() const
{
    return m_polyFat;
}

double FoodItem::transFat() const
{
    return m_transFat;
}

double FoodItem::carbs() const
{
    return m_carbs;
}

double FoodItem::fiber() const
{
    return m_fiber;
}

double FoodItem::sugar() const
{
    return m_sugar;
}

double FoodItem::addedSugar() const
{
    return m_addedSugar;
}

double FoodItem::protein() const
{
    return m_protein;
}

double FoodItem::cholesterol() const
{
    return m_cholesterol;
}

double FoodItem::calcium() const
{
    return m_calcium;
}

double FoodItem::iron() const
{
    return m_iron;
}

double FoodItem::sodium() const
{
    return m_sodium;
}

double FoodItem::potassium() const
{
    return m_potassium;
}

double FoodItem::vitaminA() const
{
    return m_vitaminA;
}

double FoodItem::vitaminC() const
{
    return m_vitaminC;
}

double FoodItem::vitaminD() const
{
    return m_vitaminD;
}

QList<ServingSize> FoodItem::servingSizes() const
{
    return m_servingSizes;
}

FoodItem::FoodItem(const QJsonObject &obj) {
    QJsonObject item = obj.value("item").toObject();
    if (item.empty()) {
        item = obj;
    }

    m_brand = item.value("brand_name").toString("Unknown");
    m_name = item.value("description").toString("Unknown Food");

    m_id = item.value("id").toVariant().toString();

    QJsonObject nutrition = item.value("nutritional_contents").toObject();

    auto getValue = [nutrition](const QString &field) -> double {
        return nutrition.value(field).toDouble();
    };

    m_fat = getValue("fat");
    m_satFat = getValue("saturated_fat");
    m_monoFat = getValue("monounsaturated_fat");
    m_polyFat = getValue("polyunsaturated_fat");
    m_transFat = getValue("trans_fat");

    m_carbs = getValue("carbohydrates");
    m_fiber = getValue("fiber");
    m_sugar = getValue("sugar");
    m_addedSugar = getValue("added_sugars");

    m_protein = getValue("protein");
    m_cholesterol = getValue("cholesterol");

    m_calcium = getValue("calcium");
    m_iron = getValue("iron");
    m_sodium = getValue("sodium");
    m_potassium = getValue("potassium");

    m_vitaminA = getValue("vitamin_a");
    m_vitaminC = getValue("vitamin_c");
    m_vitaminD = getValue("vitamin_d");

    // Serving Sizes
    QJsonArray servings = item.value("serving_sizes").toArray();

    for (QJsonValueRef ref : servings) {
        QJsonObject obj = ref.toObject();

        double mult = obj.value("nutrition_multiplier").toDouble();
        QString unit = obj.value("unit").toString("g");
        double value = obj.value("value").toDouble();

        ServingSize size(mult, unit, value);

        m_servingSizes.append(size);
    }
}

FoodItem::FoodItem(const QString &html) {
    QString split = html.split("<script id=\"__NEXT_DATA__\" type=\"application/json\">").at(1).split("</script>").at(0);

    QJsonDocument doc = QJsonDocument::fromJson(split.toUtf8());

    // behind like 30 layers you can actually find the original json in the html
    QJsonObject dehydratedState = doc.object().value("props").toObject().value("pageProps").toObject().value("dehydratedState").toObject();
    QJsonObject query = dehydratedState.value("queries").toArray().at(0).toObject();

    QJsonObject foodData = query.value("state").toObject().value("data").toObject().value("data").toObject().value("foodByObfuscatedId").toObject();
    QJsonArray queryKey = query.value("queryKey").toArray();

    m_brand = foodData.value("brand").toString("Unknown");
    m_name = foodData.value("description").toString("Unknown Food");

    m_id = dehydratedState.value("foodID").toVariant().toString();

    QList<ServingSize> sizes;

    QJsonArray servings = foodData.value("servingSizes").toArray();

    double defaultCalories = std::nan(0);

    for (QJsonValueRef ref : servings) {
        QJsonObject obj = ref.toObject();

        double value = obj.value("value").toDouble();
        QString unit = obj.value("unit").toString("serving");
        double base = 1.0;

        QJsonObject nutrition = obj.value("nutrition").toObject();
        double cals = nutrition.value("energy").toObject().value("value").toDouble();
        if (std::isnan(defaultCalories)) {
            defaultCalories = cals;

            auto getValue = [nutrition](const QString &field) -> double {
                return nutrition.value(field).toDouble();
            };

            m_fat = getValue("fat");
            m_satFat = getValue("saturatedFat");
            m_monoFat = getValue("monounsaturatedFat");
            m_polyFat = getValue("polyunsaturatedFat");
            m_transFat = getValue("transFat");

            m_carbs = getValue("carbs");
            m_fiber = getValue("fiber");
            m_sugar = getValue("sugar");
            m_addedSugar = 0.0;

            m_protein = getValue("protein");
            m_cholesterol = getValue("cholesterol");

            m_calcium = getValue("calcium");
            m_iron = getValue("iron");
            m_sodium = getValue("sodium");
            m_potassium = getValue("potassium");

            m_vitaminA = getValue("vitaminA");
            m_vitaminC = getValue("vitaminC");
            m_vitaminD = getValue("vitaminD");
        } else {
            base = cals / defaultCalories;
        }

        ServingSize size(base, unit, value);
        sizes.append(size);
    }

    m_servingSizes = sizes;
}
