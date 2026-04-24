include <QCoreApplication>
#include <QDebug>

#include "DatabaseManager.h"
#include "ClientManager.h"
#include "VehicleManager.h"
#include "ReservationManager.h"
#include "LoginManager.h"
#include "AuditLogger.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // ── 1. DATABASE ────────────────────────────────────────────────────────
    DatabaseManager& db = DatabaseManager::instance();
    if (!db.connect("biblio.db")) {
        qCritical() << "Connexion échouée.";
        return 1;
    }
    if (!db.initTables()) {
        qCritical() << "Init tables échouée.";
        return 1;
    }
    AuditLogger::log("Database connectée et tables initialisées", "INFO");

    // ── 2. CLIENT ──────────────────────────────────────────────────────────
    ClientManager clientMgr;

    Client c;
    c.name  = "Ahmed Alami";
    c.cin   = "BE123456";
    c.phone = "0612345678";
    c.email = "ahmed@email.com";
    clientMgr.add(c);
    AuditLogger::log("Client ajouté : Ahmed Alami", "INFO");

    auto clients = clientMgr.getAll();
    qDebug() << "Clients en base :" << clients.size();

    // ── 3. VEHICLE ─────────────────────────────────────────────────────────
    VehicleManager vehicleMgr;

    Vehicle v;
    v.brand     = "Dacia";
    v.model     = "Logan";
    v.plate     = "12345-A-1";
    v.available = true;
    v.dailyRate = 250.0;
    vehicleMgr.add(v);
    AuditLogger::log("Véhicule ajouté : Dacia Logan", "INFO");

    auto vehicles = vehicleMgr.getAll();
    qDebug() << "Véhicules en base :" << vehicles.size();

    // ── 4. RESERVATION ─────────────────────────────────────────────────────
    ReservationManager resMgr;

    if (!clients.empty() && !vehicles.empty()) {
        Reservation r;
        r.clientId  = clients[0].id;
        r.vehicleId = vehicles[0].id;
        r.startDate = QDate::currentDate();
        r.endDate   = QDate::currentDate().addDays(3);
        r.status    = "active";
        resMgr.add(r);
        AuditLogger::log("Réservation créée", "INFO");
    }

    auto reservations = resMgr.getAll();
    qDebug() << "Réservations en base :" << reservations.size();

    // ── 5. LOGIN ───────────────────────────────────────────────────────────
    // D'abord insérer un user test directement en DB
    QSqlQuery q(db.getDb());
    q.exec("INSERT OR IGNORE INTO users (username, password) "
           "VALUES ('admin', 'admin123');");

    LoginManager loginMgr;

    // Test login correct
    auto user = loginMgr.login("admin", "admin123");
    if (user.has_value()) {
        qDebug() << "Login OK — bienvenue :" << user->username;
        AuditLogger::log("Login réussi : admin", "INFO");
    }

    // Test login incorrect
    auto userFail = loginMgr.login("admin", "mauvais_mdp");
    if (!userFail.has_value()) {
        qDebug() << "Login échoué — mot de passe incorrect.";
        AuditLogger::log("Echec login : admin", "ERROR");
    }

    loginMgr.logout(user->id);
    AuditLogger::log("Logout : admin", "INFO");

    qDebug() << "Vérifier audit.log pour les logs.";
    return 0;
}
