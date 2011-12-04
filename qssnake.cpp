#include "qssnake.hpp"
#include <QMenu>
#include <QStatusBar>
#include <QMenuBar>
#include <QIcon>
#include <QToolBar>
#include <QString>
#include <QPainter>
#include <QLayout>
#include <iostream>
#include <ctime>
using namespace std;

QSSnake::QSSnake() : QMainWindow(0, 0) {
	resize(600, 400);
	setMinimumSize(600, 400);
	setMaximumSize(600, 400);
	setWindowTitle("QSSnake");
	statusBar()->showMessage("Welcome in QSSnake...");
	statusBar()->show();

	setStatusTip("QSSnake v0.1");

	QAction* new_game = new QAction("&New game", this);
	new_game->setIcon(QIcon::fromTheme("media-playback-start"));
	new_game->setStatusTip("Start new game");
	QAction* quit = new QAction("&Quit", this);
	quit->setIcon(QIcon::fromTheme("application-exit"));
	quit->setStatusTip("Close game");

	QMenu* game = menuBar()->addMenu("&Game");
	game->addAction(new_game);
	game->addAction(quit);

	QToolBar* tool_bar = addToolBar("Main toolbar");
	tool_bar->addAction(new_game);
	tool_bar->addAction(quit);
	tool_bar->show();

	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(new_game, SIGNAL(triggered()), this, SLOT(startGame()));

	canvas = new Canvas(this);
	setCentralWidget(canvas);

	srand(time(NULL));
}

void QSSnake::startGame() {
	canvas->initGame();
	canvas->startGame();
}

void QSSnake::keyPressEvent(QKeyEvent* event) {
	canvas->keyPressEvent(event);
}

QSSnake::Canvas::Canvas(QWidget* parent) : QWidget(parent) {
	snake = NULL;
	timerId = -1;
	in_game = false;
}

void QSSnake::Canvas::initGame() {
	if(timerId > 0)
		killTimer(timerId);
	direction = 0x00;
	dots_size = 8;
	in_game = false;
	timerId = startTimer(80);
	snake_size = 1;
	max_dots = (width() / dots_size) * (height() * dots_size);
	if(snake != NULL)
		delete[] snake;
	snake = new QPoint[max_dots];
	snake[0].setX(((width() / dots_size) / 2) * dots_size);
	snake[0].setY(((height() / dots_size) / 2) * dots_size);
	locateFood();
}

void QSSnake::Canvas::paintEvent(QPaintEvent* event) {
	if(!in_game)
		return;
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	for(int i = 0; i < snake_size; ++i) {
		if(i == 0) {
			painter.setBrush(QBrush("#FF0000"));
			painter.setPen(QColor("#FF0000"));
		} else {
			painter.setBrush(QBrush("#00AD01"));
			painter.setPen(QColor("#00AD01"));
		}
		painter.drawEllipse(snake[i].x(), snake[i].y(), dots_size, dots_size);
	}
	painter.setBrush(QBrush("#C7B100"));
	painter.setPen(QColor("#C7B100"));
	painter.drawEllipse(food.x(), food.y(), dots_size, dots_size);
}

void QSSnake::Canvas::keyPressEvent(QKeyEvent* event) {
	switch(event->key()) {
		case Qt::Key_Up: {
			if(direction == 0x04 && snake_size > 1)
				return;
			direction = 0x01;	
		}
		break;
		case Qt::Key_Right: {
			if(direction == 0x08 && snake_size > 1)
				return;
			direction = 0x02;
		}
		break;
		case Qt::Key_Down: {
			if(direction == 0x01 && snake_size > 1)
				return;
			direction = 0x04;
		}
		break;
		case Qt::Key_Left: {
			if(direction == 0x02 && snake_size > 1)
				return;
			direction = 0x08;
		}
		break;
	}
}

void QSSnake::Canvas::timerEvent(QTimerEvent* event) {
	moveSnake();
	checkCollisions();
	repaint();
}

void QSSnake::Canvas::moveSnake() {
	if(direction == 0)
		return;
	for(int i = snake_size - 1; i > 0; --i) {
		snake[i].setX(snake[i - 1].x());
		snake[i].setY(snake[i - 1].y());
	}
	switch(direction) {
		case 0x01:
			snake[0] += QPoint(0, -1 * dots_size);
		break;
		case 0x02:
			snake[0] += QPoint(dots_size, 0);
		break;
		case 0x04:
			snake[0] += QPoint(0, dots_size);
		break;
		case 0x08:
			snake[0] += QPoint(-1 * dots_size, 0);
		break;
	}
	if(snake[0].x() > width())
		snake[0].setX(0);
	if(snake[0].x() < 0)
		snake[0].setX((width() / dots_size) * dots_size);
	if(snake[0].y() > height())
		snake[0].setY(0);
	if(snake[0].y() < 0)
		snake[0].setY((height() / dots_size) * dots_size);
}

void QSSnake::Canvas::startGame() {
	in_game = true;
}

void QSSnake::Canvas::locateFood() {
	food.setX((rand() % (width() / dots_size)) * dots_size);
	food.setY((rand() % (height() / dots_size)) * dots_size);
}

void QSSnake::Canvas::checkCollisions() {
	if(food.x() == snake[0].x() && food.y() == snake[0].y()) {
		snake[snake_size] = QPoint(-100, -100);
		++snake_size;
		locateFood();
	}
	for(int i = 1; i < snake_size; ++i) {
		if(snake[i] == snake[0]) {
			initGame();
			in_game = true;
			break;
		}
	}
}
