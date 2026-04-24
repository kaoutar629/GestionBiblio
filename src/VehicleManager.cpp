#include "VehicleManager.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void VehicleManager::add(const Vehicle& v) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "INSERT INTO vehicles (brand, model, plate, available, daily_rate) "
        "VALUES (:brand, :model, :plate, :available, :daily_rate);"
    );
    q.bindValue(":brand",      v.brand);
    q.bindValue(":model",      v.model);
    q.bindValue(":plate",      v.plate);
    q.bindValue(":available",  v.available ? 1 : 0);
    q.bindValue(":daily_rate", v.dailyRate);

    if (!q.exec())
        qWarning() << "[VehicleManager::add] Erreur :" << q.lastError().text();
    else
        qDebug() << "[VehicleManager::add] Véhicule ajouté, id =" << q.lastInsertId().toInt();
}

bool VehicleManager::remove(int id) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare("DELETE FROM vehicles WHERE id = :id;");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "[VehicleManager::remove] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool VehicleManager::update(const Vehicle& v) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "UPDATE vehicles SET brand=:brand, model=:model, plate=:plate, "
        "available=:available, daily_rate=:daily_rate WHERE id = :id;"
    );
    q.bindValue(":brand",      v.brand);
    q.bindValue(":model",      v.model);
    q.bindValue(":plate",      v.plate);
    q.bindValue(":available",  v.available ? 1 : 0);
    q.bindValue(":daily_rate", v.dailyRate);
    q.bindValue(":id",         v.id);

    if (!q.exec()) {
        qWarning() << "[VehicleManager::update] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

std::optional<Vehicle> VehicleManager::getById(int id) const {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "SELECT id, brand, model, plate, available, daily_rate "
        "FROM vehicles WHERE id = :id;"
    );
    q.bindValue(":id", id);

    if (!q.exec() || !q.next()) {
        qWarning() << "[VehicleManager::getById] Non trouvé, id =" << id;
        return std::nullopt;
    }

    Vehicle v;
    v.id        = q.value(0).toInt();
    v.brand     = q.value(1).toString();
    v.model     = q.value(2).toString();
    v.plate     = q.value(3).toString();
    v.available = q.value(4).toInt() == 1;
    v.dailyRate = q.value(5).toDouble();
    return v;
}

std::vector<Vehicle> VehicleManager::getAll() const {
    std::vector<Vehicle> vehicles;
    QSqlQuery q(DatabaseManager::instance().getDb());

    if (!q.exec("SELECT id, brand, model, plate, available, daily_rate FROM vehicles;")) {
        qWarning() << "[VehicleManager::getAll] Erreur :" << q.lastError().text();
        return vehicles;
    }

    while (q.next()) {
        Vehicle v;
        v.id        = q.value(0).toInt();
        v.brand     = q.value(1).toString();
        v.model     = q.value(2).toString();
        v.plate     = q.value(3).toString();
        v.available = q.value(4).toInt() == 1;
        v.dailyRate = q.value(5).toDouble();
        vehicles.push_back(v);
    }
    return vehicles;
}
