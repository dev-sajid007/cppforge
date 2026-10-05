#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QSqlDatabase>

class Database
{
public:
    static bool connect();

private:
    Database() = default;
};

#endif