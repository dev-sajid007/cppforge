#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "User.hpp"

#include <QList>
#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QLineEdit* searchInput;

    QPushButton* addButton;

    QTableWidget* table;

    void loadUsers();

    void populateTable(const QList<User>& users);

    bool showUserDialog(const QString& title, User& user);

    void addUser();

    void editUser(int id);

    void deleteUser(int id);
};

#endif
