#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

bool DatabaseManager::connect(const QString& dbPath) {
    if (db_.isOpen())
        return true;

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbPath);

    if (!db_.open()) {
        qWarning() << "[DatabaseManager] Connexion échouée :" << db_.lastError().text();
        return false;
    }

    qDebug() << "[DatabaseManager] Connecté à" << dbPath;
    return true;
}

bool DatabaseManager::isConnected() const {
    return db_.isOpen();
}

QSqlDatabase& DatabaseManager::getDb() {
    return db_;
}

bool DatabaseManager::initTables() {
    QSqlQuery q(db_);

    // Table clients
    bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS clients ("
        "  id    INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name  TEXT    NOT NULL,"
        "  cin   TEXT    UNIQUE NOT NULL,"
        "  phone TEXT,"
        "  email TEXT"
        ");"
    );
    if (!ok) {
        qWarning() << "[DatabaseManager] Erreur création table clients :" << q.lastError().text();
        return false;
    }

    // Table vehicles
    ok = q.exec(
        "CREATE TABLE IF NOT EXISTS vehicles ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  brand      TEXT    NOT NULL,"
        "  model      TEXT    NOT NULL,"
        "  plate      TEXT    UNIQUE NOT NULL,"
        "  available  INTEGER NOT NULL DEFAULT 1,"
        "  daily_rate REAL    NOT NULL DEFAULT 0.0"
        ");"
    );
    if (!ok) {
        qWarning() << "[DatabaseManager] Erreur création table vehicles :" << q.lastError().text();
        return false;
    }

    // Table reservations
    ok = q.exec(
        "CREATE TABLE IF NOT EXISTS reservations ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  client_id  INTEGER NOT NULL REFERENCES clients(id),"
        "  vehicle_id INTEGER NOT NULL REFERENCES vehicles(id),"
        "  start_date TEXT    NOT NULL,"
        "  end_date   TEXT    NOT NULL,"
        "  status     TEXT    NOT NULL DEFAULT 'active'"
        ");"
    );
    if (!ok) {
        qWarning() << "[DatabaseManager] Erreur création table reservations :" << q.lastError().text();
        return false;
    }

    // Table users
    ok = q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT    UNIQUE NOT NULL,"
        "  password TEXT    NOT NULL"
        ");"
    );
    if (!ok) {
        qWarning() << "[DatabaseManager] Erreur création table users :" << q.lastError().text();
        return false;
    }

    qDebug() << "[DatabaseManager] Tables initialisées avec succès.";
    return true;
}
