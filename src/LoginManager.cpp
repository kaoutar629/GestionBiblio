#include "LoginManager.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

std::optional<User> LoginManager::login(const std::string& username,
                                         const std::string& password) {
    QSqlQuery q(DatabaseManager::instance().getDb());
    q.prepare("SELECT id, username, password FROM users "
              "WHERE username = :username;");
    q.bindValue(":username", QString::fromStdString(username));

    if (!q.exec() || !q.next()) {
        qWarning() << "[LoginManager] User non trouvé :" << username;
        return std::nullopt;
    }

    QString dbPassword = q.value(2).toString();
    if (dbPassword != QString::fromStdString(password)) {
        qWarning() << "[LoginManager] Mot de passe incorrect pour :" << username;
        return std::nullopt;
    }

    User u;
    u.id       = q.value(0).toInt();
    u.username = q.value(1).toString();
    u.password = dbPassword;
    return u;
}

void LoginManager::logout(int userId) {
    qDebug() << "[LoginManager] User déconnecté, id =" << userId;
}
