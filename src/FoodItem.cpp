#include "FoodItem.h"

#include <QJsonArray>
#include <QJsonDocument>

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

NutrientUnion FoodItem::nutrients() const
{
    return m_nutrients;
}

QList<ServingSize> FoodItem::servingSizes() const
{
    return m_servingSizes;
}

QString FoodItem::id() const
{
    return m_id;
}

bool operator==(const FoodItem &a, const FoodItem &b)
{
    return a.id() == b.id();
}

FoodItem::FoodItem(const QJsonObject &obj) {
    m_brand = obj.value("brand").toString("Unknown");
    m_name = obj.value("name").toString("Unknown Food");

    m_id = obj.value("id").toVariant().toString();

    auto getValue = [obj](const QString &field) -> double {
        return obj.value(field).toDouble();
    };

    NutrientUnion n;

    n.fat = getValue("fat");
    n.satFat = getValue("sat");
    n.monoFat = getValue("mono");
    n.polyFat = getValue("poly");
    n.transFat = getValue("trans");

    n.carbs = getValue("carbs");
    n.fiber = getValue("fiber");
    n.sugar = getValue("sugar");
    n.addedSugar = getValue("added_sugars");

    n.protein = getValue("protein");
    n.cholesterol = getValue("cholesterol");

    n.calcium = getValue("calcium");
    n.iron = getValue("iron");
    n.sodium = getValue("sodium");
    n.potassium = getValue("potassium");

    n.vitaminA = getValue("vitaminA");
    n.vitaminC = getValue("vitaminC");
    n.vitaminD = getValue("vitaminD");

    m_nutrients = n;

    // Serving Sizes
    QJsonArray servings = obj.value("servings").toArray();

    for (QJsonValueRef ref : servings) {
        QJsonObject obj = ref.toObject();

        double mult = obj.value("mult").toDouble();
        QString unit = obj.value("unit").toString("g");
        double value = obj.value("value").toDouble();

        ServingSize size(mult, unit, value);

        m_servingSizes.append(size);
    }
}

QJsonObject FoodItem::toJson() const
{
    QJsonObject obj{};

    obj.insert("brand", m_brand);
    obj.insert("name", m_name);
    obj.insert("id", m_id);

    auto setValue = [&obj](const QString &field, double value) mutable {
        obj.insert(field, value);
    };

    setValue("fat", m_nutrients.fat);
    setValue("sat", m_nutrients.satFat);
    setValue("mono", m_nutrients.monoFat);
    setValue("poly", m_nutrients.polyFat);
    setValue("trans", m_nutrients.transFat);

    setValue("carbs", m_nutrients.carbs);
    setValue("fiber", m_nutrients.fiber);
    setValue("sugar", m_nutrients.sugar);
    setValue("added_sugars", m_nutrients.addedSugar);

    setValue("protein", m_nutrients.protein);
    setValue("cholesterol", m_nutrients.cholesterol);

    setValue("calcium", m_nutrients.calcium);
    setValue("iron", m_nutrients.iron);
    setValue("sodium", m_nutrients.sodium);
    setValue("potassium", m_nutrients.potassium);

    setValue("vitaminA", m_nutrients.vitaminA);
    setValue("vitaminC", m_nutrients.vitaminC);
    setValue("vitaminD", m_nutrients.vitaminD);

    // Serving Sizes
    QJsonArray servings{};

    for (const ServingSize &size : m_servingSizes) {
        QJsonObject serving{};

        serving.insert("mult", size.baseMultiplier());
        serving.insert("unit", size.baseUnit());
        serving.insert("value", size.defaultValue());

        servings.append(serving);
    }

    obj.insert("servings", servings);

    return obj;
}

// FoodItem::FoodItem(const QString &html) {
//     QString split = html.split("<script id=\"__NEXT_DATA__\" type=\"application/json\">").at(1).split("</script>").at(0);

//     QJsonDocument doc = QJsonDocument::fromJson(split.toUtf8());

//     // behind like 30 layers you can actually find the original json in the html
//     QJsonObject dehydratedState = doc.object().value("props").toObject().value("pageProps").toObject().value("dehydratedState").toObject();
//     QJsonObject query = dehydratedState.value("queries").toArray().at(0).toObject();

//     QJsonObject foodData = query.value("state").toObject().value("data").toObject().value("data").toObject().value("foodByObfuscatedId").toObject();
//     QJsonArray queryKey = query.value("queryKey").toArray();

//     m_brand = foodData.value("brand").toString("Unknown");
//     m_name = foodData.value("description").toString("Unknown Food");

//     m_id = dehydratedState.value("foodID").toVariant().toString();

//     QList<ServingSize> sizes;

//     QJsonArray servings = foodData.value("servingSizes").toArray();

//     double defaultCalories = std::nan(0);

//     for (QJsonValueRef ref : servings) {
//         QJsonObject obj = ref.toObject();

//         double value = obj.value("value").toDouble();
//         QString unit = obj.value("unit").toString("serving");
//         double base = 1.0;

//         QJsonObject nutrition = obj.value("nutrition").toObject();
//         double cals = nutrition.value("energy").toObject().value("value").toDouble();
//         if (std::isnan(defaultCalories)) {
//             defaultCalories = cals;

//             auto getValue = [nutrition](const QString &field) -> double {
//                 return nutrition.value(field).toDouble();
//             };

//             m_fat = getValue("fat");
//             m_satFat = getValue("saturatedFat");
//             m_monoFat = getValue("monounsaturatedFat");
//             m_polyFat = getValue("polyunsaturatedFat");
//             m_transFat = getValue("transFat");

//             m_carbs = getValue("carbs");
//             m_fiber = getValue("fiber");
//             m_sugar = getValue("sugar");
//             m_addedSugar = 0.0;

//             m_protein = getValue("protein");
//             m_cholesterol = getValue("cholesterol");

//             m_calcium = getValue("calcium");
//             m_iron = getValue("iron");
//             m_sodium = getValue("sodium");
//             m_potassium = getValue("potassium");

//             m_vitaminA = getValue("vitaminA");
//             m_vitaminC = getValue("vitaminC");
//             m_vitaminD = getValue("vitaminD");
//         } else {
//             base = cals / defaultCalories;
//         }

//         ServingSize size(base, unit, value);
//         sizes.append(size);
//     }

//     m_servingSizes = sizes;
// }

bool operator==(const FoodServing &a, const FoodServing &b) {
    return a.item == b.item
           && a.size == b.size
           && a.units == b.units;
}

QJsonObject FoodServing::toJson() const
{
    QJsonObject obj;

    obj.insert("item", item.toJson());
    obj.insert("size", size.toJson());
    obj.insert("units", units);

    return obj;
}

FoodServing FoodServing::fromJson(const QJsonObject &obj)
{
    FoodServing food;

    FoodItem item = FoodItem(obj.value("item").toObject());
    ServingSize size = ServingSize::fromJson(obj.value("size").toObject());
    double units = obj.value("units").toDouble();

    food = FoodServing{item, size, units};

    return food;
}
