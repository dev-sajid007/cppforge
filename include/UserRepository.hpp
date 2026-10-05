#ifndef USERREPOSITORY_HPP
#define USERREPOSITORY_HPP

#include "User.hpp"

#include <QList>

#include <optional>

class UserRepository
{
public:
    bool create(const User& user);

    QList<User> getAll();

    std::optional<User> findById(int id);

    bool update(const User& user);

    bool remove(int id);

    QList<User> search(const QString& keyword);
};

#endif
