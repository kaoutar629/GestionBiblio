#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool connect(const QString& dbPath = "biblio.db");
    bool isConnected() const;
    bool initTables();
    QSqlDatabase& getDb();

private:
    DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

    QSqlDatabase db_;
};

#endif

