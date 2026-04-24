#include "ReservationManager.h"

#include "ReservationManager.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

static Reservation rowToReservation(QSqlQuery& q) {
    Reservation r;
    r.id        = q.value(0).toInt();
    r.clientId  = q.value(1).toInt();
    r.vehicleId = q.value(2).toInt();
    r.startDate = QDate::fromString(q.value(3).toString(), "yyyy-MM-dd");
    r.endDate   = QDate::fromString(q.value(4).toString(), "yyyy-MM-dd");
    r.status    = q.value(5).toString();
    return r;
}

void ReservationManager::add(const Reservation& item) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "INSERT INTO reservations (client_id, vehicle_id, start_date, end_date, status) "
        "VALUES (:cid, :vid, :start, :end, :status);"
    );
    q.bindValue(":cid",    item.clientId);
    q.bindValue(":vid",    item.vehicleId);
    q.bindValue(":start",  item.startDate.toString("yyyy-MM-dd"));
    q.bindValue(":end",    item.endDate.toString("yyyy-MM-dd"));
    q.bindValue(":status", item.status.isEmpty() ? "active" : item.status);

    if (!q.exec())
        qWarning() << "[ReservationManager::add] Erreur :" << q.lastError().text();
    else
        qDebug() << "[ReservationManager::add] Réservation créée, id =" << q.lastInsertId().toInt();
}

bool ReservationManager::remove(int id) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare("DELETE FROM reservations WHERE id = :id;");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "[ReservationManager::remove] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool ReservationManager::update(const Reservation& item) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "UPDATE reservations SET client_id=:cid, vehicle_id=:vid, "
        "start_date=:start, end_date=:end, status=:status "
        "WHERE id = :id;"
    );
    q.bindValue(":cid",    item.clientId);
    q.bindValue(":vid",    item.vehicleId);
    q.bindValue(":start",  item.startDate.toString("yyyy-MM-dd"));
    q.bindValue(":end",    item.endDate.toString("yyyy-MM-dd"));
    q.bindValue(":status", item.status);
    q.bindValue(":id",     item.id);

    if (!q.exec()) {
        qWarning() << "[ReservationManager::update] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

std::optional<Reservation> ReservationManager::getById(int id) const {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "SELECT id, client_id, vehicle_id, start_date, end_date, status "
        "FROM reservations WHERE id = :id;"
    );
    q.bindValue(":id", id);

    if (!q.exec() || !q.next()) {
        qWarning() << "[ReservationManager::getById] Non trouvé, id =" << id;
        return std::nullopt;
    }
    return rowToReservation(q);
}

std::vector<Reservation> ReservationManager::getAll() const {
    std::vector<Reservation> list;
    QSqlQuery q(DatabaseManager::instance().getDb());

    if (!q.exec("SELECT id, client_id, vehicle_id, start_date, end_date, status FROM reservations;")) {
        qWarning() << "[ReservationManager::getAll] Erreur :" << q.lastError().text();
        return list;
    }

    while (q.next())
        list.push_back(rowToReservation(q));

    return list;
}
