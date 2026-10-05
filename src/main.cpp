#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    window.setWindowTitle("C++ User CRUD");
    window.resize(900, 600);

    auto *label = new QLabel("User Management");

    label->setAlignment(Qt::AlignCenter);

    window.setCentralWidget(label);

    window.show();

    return app.exec();
}