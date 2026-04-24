#include "ClientManager.h"

#include "ClientManager.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void ClientManager::add(const Client& c) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "INSERT INTO clients (name, cin, phone, email) "
        "VALUES (:name, :cin, :phone, :email);"
    );
    q.bindValue(":name",  c.name);
    q.bindValue(":cin",   c.cin);
    q.bindValue(":phone", c.phone);
    q.bindValue(":email", c.email);

    if (!q.exec())
        qWarning() << "[ClientManager::add] Erreur :" << q.lastError().text();
    else
        qDebug() << "[ClientManager::add] Client ajouté, id =" << q.lastInsertId().toInt();
}

bool ClientManager::remove(int id) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare("DELETE FROM clients WHERE id = :id;");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "[ClientManager::remove] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool ClientManager::update(const Client& c) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare(
        "UPDATE clients SET name=:name, cin=:cin, phone=:phone, email=:email "
        "WHERE id = :id;"
    );
    q.bindValue(":name",  c.name);
    q.bindValue(":cin",   c.cin);
    q.bindValue(":phone", c.phone);
    q.bindValue(":email", c.email);
    q.bindValue(":id",    c.id);

    if (!q.exec()) {
        qWarning() << "[ClientManager::update] Erreur :" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

std::optional<Client> ClientManager::getById(int id) const {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare("SELECT id, name, cin, phone, email FROM clients WHERE id = :id;");
    q.bindValue(":id", id);

    if (!q.exec() || !q.next()) {
        qWarning() << "[ClientManager::getById] Non trouvé, id =" << id;
        return std::nullopt;
    }

    Client c;
    c.id    = q.value(0).toInt();
    c.name  = q.value(1).toString();
    c.cin   = q.value(2).toString();
    c.phone = q.value(3).toString();
    c.email = q.value(4).toString();
    return c;
}

std::vector<Client> ClientManager::getAll() const {
    std::vector<Client> clients;
    QSqlQuery q(DatabaseManager::instance().getDb());

    if (!q.exec("SELECT id, name, cin, phone, email FROM clients;")) {
        qWarning() << "[ClientManager::getAll] Erreur :" << q.lastError().text();
        return clients;
    }

    while (q.next()) {
        Client c;
        c.id    = q.value(0).toInt();
        c.name  = q.value(1).toString();
        c.cin   = q.value(2).toString();
        c.phone = q.value(3).toString();
        c.email = q.value(4).toString();
        clients.push_back(c);
    }
    return clients;
}
