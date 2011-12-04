#include <iostream>
#include "qssnake.hpp"
using namespace std;

int main(int argc, char** argv) {
	QApplication main_app(argc, argv);
	QSSnake qssnake;
	qssnake.show();
	return main_app.exec();
}
