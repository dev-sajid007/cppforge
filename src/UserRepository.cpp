#include "UserRepository.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace
{
    User userFromQuery(const QSqlQuery& query)
    {
        User user;

        user.id = query.value("id").toInt();
        user.name = query.value("name").toString();
        user.email = query.value("email").toString();

        return user;
    }
}

bool UserRepository::create(const User& user)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO users (name, email) "
        "VALUES (:name, :email)"
    );

    query.bindValue(":name", user.name);
    query.bindValue(":email", user.email);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QList<User> UserRepository::getAll()
{
    QList<User> users;

    QSqlQuery query;

    if (!query.exec("SELECT id, name, email FROM users ORDER BY id")) {
        qDebug() << query.lastError().text();
        return users;
    }

    while (query.next()) {
        users.append(userFromQuery(query));
    }

    return users;
}

std::optional<User> UserRepository::findById(int id)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, name, email FROM users "
        "WHERE id = :id"
    );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return std::nullopt;
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return userFromQuery(query);
}

bool UserRepository::update(const User& user)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE users "
        "SET name = :name, email = :email "
        "WHERE id = :id"
    );

    query.bindValue(":name", user.name);
    query.bindValue(":email", user.email);
    query.bindValue(":id", user.id);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool UserRepository::remove(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM users WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QList<User> UserRepository::search(const QString& keyword)
{
    QList<User> users;

    QSqlQuery query;

    query.prepare(
        "SELECT id, name, email FROM users "
        "WHERE name LIKE :keyword OR email LIKE :keyword "
        "ORDER BY id"
    );

    query.bindValue(":keyword", "%" + keyword + "%");

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return users;
    }

    while (query.next()) {
        users.append(userFromQuery(query));
    }

    return users;
}
