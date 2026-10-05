#include "Database.hpp"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QDir dataDir(QDir::currentPath() + "/data");

    if (!dataDir.exists() && !dataDir.mkpath(".")) {
        qDebug() << "Failed to create data directory:"
                 << dataDir.path();

        return false;
    }

    db.setDatabaseName(dataDir.filePath("users.db"));

    if (!db.open()) {
        qDebug() << "Database error:"
                 << db.lastError().text();

        return false;
    }

    QSqlQuery query;

    bool created = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "email TEXT NOT NULL UNIQUE"
        ")"
    );

    if (!created) {
        qDebug() << "Failed to create users table:"
                 << query.lastError().text();

        return false;
    }

    return true;
}
