#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    window.setWindowTitle("C++ User CRUD");
    window.resize(900, 600);

    QWidget centralWidget;

    window.setCentralWidget(&centralWidget);

    QVBoxLayout layout;

    layout.setParent(&centralWidget);

    QLabel title("User Management");

    layout.addWidget(&title);

    window.show();

    return app.exec();
}