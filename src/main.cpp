#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    window.setWindowTitle("C++ User CRUD");
    window.resize(900, 600);

    QWidget centralWidget;

    window.setCentralWidget(&centralWidget);

    QVBoxLayout layout;

    centralWidget.setLayout(&layout);

    QLabel title("User Management");

    layout.addWidget(&title);

    QPushButton addButton("Add User");

    layout.addWidget(&addButton);

    QObject::connect(
        &addButton,
        &QPushButton::clicked,
        []() {
            qDebug() << "Add User clicked!";
        }
    );

    window.show();

    return app.exec();
}