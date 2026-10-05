#include "Database.hpp"
#include "MainWindow.hpp"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!Database::connect()) {
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Could not open the user database."
        );

        return 1;
    }

    MainWindow window;

    window.show();

    return app.exec();
}
