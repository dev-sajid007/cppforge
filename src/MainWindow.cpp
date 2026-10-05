#include "MainWindow.hpp"

#include "UserRepository.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QRegularExpression>
#include <QWidget>

namespace
{
    bool isValidEmail(const QString& email)
    {
        static const QRegularExpression pattern(
            R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)"
        );

        return pattern.match(email).hasMatch();
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("C++ User CRUD");

    resize(900, 600);

    auto* central = new QWidget(this);

    setCentralWidget(central);

    auto* mainLayout = new QVBoxLayout(central);

    auto* title = new QLabel(
        "User Management"
    );

    QFont titleFont = title->font();
    titleFont.setPointSize(titleFont.pointSize() + 6);
    titleFont.setBold(true);
    title->setFont(titleFont);

    mainLayout->addWidget(title);

    auto* topLayout = new QHBoxLayout();

    searchInput = new QLineEdit();

    searchInput->setPlaceholderText(
        "Search users..."
    );

    searchInput->setClearButtonEnabled(true);

    addButton = new QPushButton(
        "+ Add User"
    );

    topLayout->addWidget(searchInput);

    topLayout->addWidget(addButton);

    mainLayout->addLayout(topLayout);

    table = new QTableWidget();

    table->setColumnCount(4);

    table->setHorizontalHeaderLabels({
        "ID",
        "Name",
        "Email",
        "Actions"
    });

    table->verticalHeader()->setVisible(false);

    table->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    table->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    table->horizontalHeader()
        ->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    table->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    table->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    mainLayout->addWidget(table);

    connect(
        addButton,
        &QPushButton::clicked,
        this,
        &MainWindow::addUser
    );

    connect(
        searchInput,
        &QLineEdit::textChanged,
        this,
        &MainWindow::loadUsers
    );

    loadUsers();
}

void MainWindow::loadUsers()
{
    UserRepository repository;

    QString keyword = searchInput->text().trimmed();

    if (keyword.isEmpty()) {
        populateTable(repository.getAll());
    } else {
        populateTable(repository.search(keyword));
    }
}

void MainWindow::populateTable(const QList<User>& users)
{
    table->setRowCount(0);

    for (const auto& user : users) {

        int row = table->rowCount();

        table->insertRow(row);

        table->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::number(user.id)
            )
        );

        table->setItem(
            row,
            1,
            new QTableWidgetItem(
                user.name
            )
        );

        table->setItem(
            row,
            2,
            new QTableWidgetItem(
                user.email
            )
        );

        auto* editButton =
            new QPushButton("Edit");

        auto* deleteButton =
            new QPushButton("Delete");

        auto* actionWidget =
            new QWidget();

        auto* actionLayout =
            new QHBoxLayout(actionWidget);

        actionLayout->setContentsMargins(
            0, 0, 0, 0
        );

        actionLayout->addWidget(editButton);

        actionLayout->addWidget(deleteButton);

        table->setCellWidget(
            row,
            3,
            actionWidget
        );

        connect(
            editButton,
            &QPushButton::clicked,
            this,
            [this, id = user.id]() {
                editUser(id);
            }
        );

        connect(
            deleteButton,
            &QPushButton::clicked,
            this,
            [this, id = user.id]() {
                deleteUser(id);
            }
        );
    }

    table->resizeRowsToContents();
}

bool MainWindow::showUserDialog(const QString& title, User& user)
{
    QDialog dialog(this);

    dialog.setWindowTitle(title);

    dialog.setMinimumWidth(400);

    auto* layout = new QVBoxLayout(&dialog);

    auto* form = new QFormLayout();

    auto* nameInput = new QLineEdit(user.name);

    auto* emailInput = new QLineEdit(user.email);

    form->addRow("Name:", nameInput);

    form->addRow("Email:", emailInput);

    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel
    );

    layout->addWidget(buttons);

    connect(
        buttons,
        &QDialogButtonBox::accepted,
        &dialog,
        [&]() {

            QString name = nameInput->text().trimmed();

            QString email = emailInput->text().trimmed();

            if (name.isEmpty()) {
                QMessageBox::warning(
                    &dialog,
                    "Invalid Input",
                    "Name is required."
                );

                nameInput->setFocus();
                return;
            }

            if (!isValidEmail(email)) {
                QMessageBox::warning(
                    &dialog,
                    "Invalid Input",
                    "Please enter a valid email address."
                );

                emailInput->setFocus();
                return;
            }

            dialog.accept();
        }
    );

    connect(
        buttons,
        &QDialogButtonBox::rejected,
        &dialog,
        &QDialog::reject
    );

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    user.name = nameInput->text().trimmed();

    user.email = emailInput->text().trimmed();

    return true;
}

void MainWindow::addUser()
{
    User user;

    if (!showUserDialog("Add User", user)) {
        return;
    }

    UserRepository repository;

    if (repository.create(user)) {

        loadUsers();

        QMessageBox::information(
            this,
            "Success",
            "User created successfully."
        );

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "Failed to create user. "
            "The email may already be in use."
        );
    }
}

void MainWindow::editUser(int id)
{
    UserRepository repository;

    auto existing = repository.findById(id);

    if (!existing) {
        QMessageBox::warning(
            this,
            "Not Found",
            "This user no longer exists."
        );

        loadUsers();
        return;
    }

    User user = *existing;

    if (!showUserDialog("Edit User", user)) {
        return;
    }

    if (repository.update(user)) {

        loadUsers();

        QMessageBox::information(
            this,
            "Success",
            "User updated successfully."
        );

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "Failed to update user. "
            "The email may already be in use."
        );
    }
}

void MainWindow::deleteUser(int id)
{
    auto answer = QMessageBox::question(
        this,
        "Delete User",
        "Are you sure you want to delete this user?"
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    UserRepository repository;

    if (repository.remove(id)) {

        loadUsers();

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "Failed to delete user."
        );
    }
}
