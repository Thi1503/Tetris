#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include "glut.h"
#include "Game.h"

using namespace std;

const int BLOCKSIZE = 40;          // Kích thước của mỗi khối
const int VPWIDTH = COLS * BLOCKSIZE;  // Chiều rộng cửa sổ hiển thị
const int VPHEIGHT = ROWS * BLOCKSIZE; // Chiều cao cửa sổ hiển thị

Game game;
GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;

// Hiển thị văn bản trên màn hình
void BitmapText(char* str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i = 0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
}

// Lấy một số ngẫu nhiên trong khoảng [0, 1]
double random()
{
	int r = rand();
	return double(r) / RAND_MAX;
}

// Hàm callback, bộ hẹn giờ
void timer(int id)
{
	if (game.killed) {	// Nếu trò chơi kết thúc

		game.paused = true;
		//glutTimerFunc(200, timer, id);
		game.clearMainGrid();
		game.clearNextPieceGrid();
		glutPostRedisplay();

	}
	else if (!game.paused) {	// Nếu trò chơi tiếp tục và không bị tạm dừng
		game.update();
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		}
		else {
			glutPostRedisplay();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Hàm callback, xử lý các hành động bàn phím
void keyboard(unsigned char key, int x, int y)
{
	if (game.paused && game.killed) {
		if (key == 13) { // 13 == ENTER
			game.killed = false;
			game.restart();
			glutTimerFunc(game.timer, timer, 0);
		}
	}
	else {
		if (key == 'p' || key == 27) { // 27 == ESCAPE
			game.paused = !game.paused;
			if (!game.paused)
				glutTimerFunc(game.timer, timer, 0);
		}
		else if (!game.paused && !game.killed && key == ' ') { // ' ' == SPACE
			game.update();
		}
	}

	glutPostRedisplay();
}

// Hàm callback, xử lý các hành động phím mũi tên
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
			glutPostRedisplay();
		}
		else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}

// Hàm callback, hiển thị cửa sổ trò chơi
void display(void)
{
	const int N = 100;
	char msg[N + 1];

	glClearColor(0.2f, 0.2f, 0.2f, 0.72); // Màu nền
	glClear(GL_COLOR_BUFFER_BIT); // Xóa bộ đệm màu

	// Lưới trò chơi
	glViewport(0, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);

	if (!game.paused) {	// Nếu trò chơi đang hoạt động
		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		// Hiển thị các ô vuông
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				Square& square = game.mainGrid[r][c];
				if (square.isFilled) {
					glColor3f(square.red, square.green, square.blue);
					glRectd(c + .1, r + .1, c + .9, r + .9);
				}
				else {
					glColor3f(0.2, 0.2, 0.2);
					glRectd(c, r, c + 1, r + 1);
				}
			}
		}

		// Vẽ các đường kẻ chia ô
		glColor3f(0.5, 0.5, 0.5); // Màu xám cho các đường kẻ
		glBegin(GL_LINES);
		// Vẽ các đường ngang
		for (int r = 0; r <= ROWS; r++) {
			glVertex2d(0, r);
			glVertex2d(COLS, r);
		}
		// Vẽ các đường dọc
		for (int c = 0; c <= COLS; c++) {
			glVertex2d(c, 0);
			glVertex2d(c, ROWS);
		}
		glEnd();
	}
	else {
		// Các trạng thái khác (PAUSE, GAME OVER)
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);
		if (game.paused && !game.killed) {
			glColor3f(1, 1, 1);
			sprintf_s(msg, N, "GAME PAUSED");
			BitmapText(msg, 140, VPHEIGHT / 2);
		}
		if (game.paused && game.killed) {
			glColor3f(1, 1, 1);
			sprintf_s(msg, N, "GAME OVER");
			BitmapText(msg, 155, VPHEIGHT / 2 + 50);
			sprintf_s(msg, N, "YOUR SCORE: %d", game.linesCleared);
			BitmapText(msg, 140, VPHEIGHT / 2);
			sprintf_s(msg, N, "Press [ENTER] to restart ...");
			BitmapText(msg, 75, VPHEIGHT / 2 - 100);
		}
	}

	// Dải phân cách dọc
	glViewport(VPWIDTH, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(1, 0);
	glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	// Thông báo bên phải màn hình
	glLoadIdentity();
	gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

	glColor3f(1, 1, 1);
	sprintf_s(msg, N, "Lines Cleared: %d", game.linesCleared);
	BitmapText(msg, 50, 100);
	sprintf_s(msg, N, "Shapes Encountered: %d", game.shapesCount);
	BitmapText(msg, 50, 50);
	sprintf_s(msg, N, "Next Shape:");
	BitmapText(msg, 50, VPHEIGHT - 50);

	// Lưới hiển thị khối tiếp theo
	glViewport(VPWIDTH + 50, -50, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, COLS, ROWS, 0);

	for (int r = 1; r < 5; r++) {
		for (int c = 0; c < 2; c++) {
			Square& square = game.nextPieceGrid[r][c];
			if (square.isFilled) {
				glColor3f(square.red, square.green, square.blue);
				glRectd(c + .1, r + .1, c + .9, r + .9);
			}
			else {
				glColor3f(0.2, 0.2, 0.2);
				glRectd(c, r, c + 1, r + 1);
			}
		}
	}

	glutSwapBuffers();
}

void main(int argc, char* argv[])
{
	srand(time(0));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH * 2, VPHEIGHT);

	glutCreateWindow("Tetris");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();
}
