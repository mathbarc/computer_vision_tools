#include "controller.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller cont;

	return a.exec();
}
