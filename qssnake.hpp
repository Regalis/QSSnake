#ifndef _QSSNAKE_HPP
#define _QSSNAKE_HPP
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QLabel>
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
				Canvas(QWidget* parent, QLabel* score_label);
				void startGame();
				void initGame();
			protected:
				bool ignore_keys;
				uint8_t direction;
				int dots_size;
				int score;
				int max_dots;
				bool in_game;
				QPoint* snake;
				QPoint food;
				QPoint bonus;
				bool bonus_in_game;
				uint8_t bonus_direction;
				int snake_size;
				int timerId;
				QLabel* score_label;
				void paintEvent(QPaintEvent* event);
				void keyPressEvent(QKeyEvent* event);
				void timerEvent(QTimerEvent* event);
				void moveSnake();
				void moveBonus();
				void locateFood();
				void initBonus();
				void checkCollisions();
				bool snakeCollision(const QPoint& p, bool skip_head = false) const;
				void updateScoreLabel();
				friend class QSSnake;
		};
		Canvas* canvas;
	protected:
		void keyPressEvent(QKeyEvent* event);
};

#endif
