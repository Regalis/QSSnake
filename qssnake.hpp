#ifndef _QSSNAKE_HPP
#define _QSSNAKE_HPP
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <stdint.h>

class QSSnake : public QMainWindow {
	Q_OBJECT

	public:
		QSSnake();
	private slots:
		void startGame();
	private:
		class Canvas : public QWidget {
			public:
				Canvas(QWidget* parent);
				void startGame();
				void initGame();
			protected:
				uint8_t direction;
				int dots_size;
				int max_dots;
				bool in_game;
				QPoint* snake;
				QPoint food;
				int snake_size;
				int timerId;
				void paintEvent(QPaintEvent* event);
				void keyPressEvent(QKeyEvent* event);
				void timerEvent(QTimerEvent* event);
				void moveSnake();
				void locateFood();
				void checkCollisions();
				friend class QSSnake;
		};
		Canvas* canvas;
	protected:
		void keyPressEvent(QKeyEvent* event);
};

#endif
