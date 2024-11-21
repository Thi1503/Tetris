#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "glut.h"
#include "Game.h"

using namespace std;

/*
Được gọi mỗi khi bộ hẹn giờ hết hạn, nếu trò chơi không kết thúc hoặc tạm dừng
*/
void Game::update() {
	// Kiểm tra va chạm của khối
	if (moveCollision(0)) {					// Nếu có va chạm
		if (activePiece.y <= 2) {			// Kiểm tra nếu va chạm kết thúc trò chơi
			killed = true;
		}
		else {								// Nếu trò chơi vẫn đang hoạt động
			updateActiveAfterCollision();	// Cố định khối tại vị trí va chạm
			checkLine();					// Kiểm tra các dòng đầy
			if (deleteLines)				// Nếu tìm thấy các dòng cần xóa
				clearLine();				// Xóa các dòng đầy
			genNextPiece();					// Tạo khối mới

			clearNextPieceGrid();
			updateNextPieceGrid();

			updateActivePiece();			// Cập nhật khối trong lưới trò chơi
		}
	}
	else {									// Nếu không có va chạm, khối sẽ rơi xuống dưới
		fixActivePiece();
		activePiece.y++;
		updateActivePiece();
	}
}

/*
Khởi động trò chơi mới và khởi tạo các phần tử cần thiết
*/
void Game::restart()
{
	clearMainGrid();		// Xóa lưới chính
	clearNextPieceGrid();	// Xóa lưới chứa khối tiếp theo
	linesCleared = 0;		// Điểm số của người chơi bằng 0
	shapesCount = 1;		// Số khối trong trò chơi được đặt là 1
	killed = false;
	paused = false;
	deleteLines = false;

	// Tạo khối hiện tại ngẫu nhiên
	activePiece = Piece(rand() % numPieces);
	activePiece.x = COLS / 2;
	activePiece.y = 0;
	updateActivePiece();

	// Tạo khối tiếp theo
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	updateNextPieceGrid();
}

/*
Cập nhật lưới trò chơi và hiển thị đúng khối khi rơi
*/
void Game::fixActivePiece() {
	// Xác định dữ liệu của khối hiện tại dựa trên loại và vị trí
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Thiết lập các ô hoạt động và không hoạt động
		square.isFilled = false;
		square.isActive = false;
	}
}

/*
Tạo khối tiếp theo trong trò chơi
*/
void Game::genNextPiece() {
	activePiece = nextPiece;
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS / 2;
	nextPiece.y = 0;
	// Tăng số lượng khối trong trò chơi
	shapesCount++;
}

/*
Di chuyển khối hiện tại sang trái hoặc phải
*/
void Game::move(int dir)
{
	if (moveCollision(dir))	// Nếu có va chạm với một trong các biên
		return;				// không có gì xảy ra
	fixActivePiece();
	activePiece.x += dir;
	updateActivePiece();
}

/*
Xóa lưới chính
*/
void Game::clearMainGrid()
{
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			mainGrid[r][c].isFilled = false;
			mainGrid[r][c].isActive = false;
		}
	}
}

/*
Xóa lưới chứa khối tiếp theo
*/
void Game::clearNextPieceGrid()
{
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			nextPieceGrid[r][c].isFilled = false;
			nextPieceGrid[r][c].isActive = false;
		}
	}
}

/*
Cập nhật vị trí của khối hiện tại khi xoay
*/
void Game::updateActivePiece() {
	// Con trỏ đến mảng chứa tất cả phép chuyển đổi
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// Tìm khối hoạt động trong lưới trò chơi
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Chuyển khối hoạt động thành các ô được lấp đầy trong lưới
		square.isFilled = true;
		square.isActive = true;
		square.red = activePiece.redVal;
		square.green = activePiece.blueVal;
		square.blue = activePiece.greenVal;
	}
}

/*
Cập nhật lưới chứa khối tiếp theo
*/
void Game::updateNextPieceGrid() {
	// Con trỏ đến mảng chứa tất cả phép chuyển đổi
	const int* transNext = nextPiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// Tìm khối hoạt động trong lưới trò chơi
		Square& squareNext = nextPieceGrid[nextPiece.y + transNext[i + 1]][nextPiece.x + transNext[i]];
		// Chuyển khối hoạt động thành các ô được lấp đầy trong lưới
		squareNext.isFilled = true;
		squareNext.isActive = true;
		squareNext.red = nextPiece.redVal;
		squareNext.green = nextPiece.blueVal;
		squareNext.blue = nextPiece.greenVal;
	}
}

/*
Hàm khởi tạo
*/
Game::Game()
{
	restart();
	timer = 500;
}

/*
Xoay khối hiện tại và kiểm tra xem khối có thể xoay hay không
*/
void Game::rotateShape(int dir) {
	// Tạo một bản sao của khối hiện tại và kiểm tra xem nó có thể xoay hay không
	activePieceCopy = Piece(rand() % numPieces);
	activePieceCopy.x = activePiece.x;
	activePieceCopy.y = activePiece.y;
	activePieceCopy.rotation = activePiece.rotation;
	activePieceCopy.type = activePiece.type;
	activePieceCopy.rotatePiece(dir);

	// Nếu khối có thể xoay, nó sẽ được xoay và hiển thị
	if (canRotate(activePieceCopy)) {
		fixActivePiece();
		activePiece.rotatePiece(dir);
		updateActivePiece();
	}
}

/*
Kiểm tra xem khối có thể xoay hay không
*/
bool Game::canRotate(Piece activeP) {
	if (rotationCollision()) {
		return false;
	}
	else
		return true;
}

/*
Kiểm tra va chạm khi xoay khối
*/
bool Game::rotationCollision() {
	int x, y;
	const int* trans = activePieceCopy.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePieceCopy.x + trans[i];
		y = activePieceCopy.y + trans[i + 1];

		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

/*
Kiểm tra va chạm khi di chuyển khối
*/
bool Game::moveCollision(int dir) {
	int x, y;
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePiece.x + trans[i];
		y = activePiece.y + trans[i + 1];
		if (dir == 0)
			y += 1;
		else
			x += dir;
		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

/*
Cập nhật vị trí của khối hiện tại sau va chạm
*/
void Game::updateActiveAfterCollision() {
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		Square& square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		square.isActive = false;
	}
}

/*
Kiểm tra dòng đầy và đánh dấu các dòng đầy để xóa
*/
void Game::checkLine() {
	int fullRows = 0;
	for (int r = 0; r < ROWS; r++) {
		bool fullRow = false;
		for (int c = 0; c < COLS; c++) {
			Square& square = mainGrid[r][c];
			if (square.isFilled) {
				fullRow = true;
			}
			else {
				fullRow = false;
				break;
			}
		}
		if (fullRow) {
			for (int c = 0; c < COLS; c++) {
				mainGrid[r][c].toBeDeleted = true;
			}
			deleteLines = true;
			linesCleared++;
		}
	}
}

/*
Xóa dòng đầy và di chuyển tất cả các dòng phía trên xuống
*/
void Game::clearLine() {
	for (int r = ROWS - 1; r > 0; r--) { // Kiểm tra từng dòng
		int linesDeleted = 0;
		if (mainGrid[r][0].toBeDeleted) {
			for (int r2 = r; r2 > 0; r2--) { // Di chuyển tất cả các dòng phía trên xuống
				for (int c = 0; c < COLS; c++) {
					mainGrid[r2][c].isFilled = mainGrid[r2 - 1][c].isFilled;
					mainGrid[r2][c].isActive = mainGrid[r2 - 1][c].isActive;
					mainGrid[r2][c].toBeDeleted = mainGrid[r2 - 1][c].toBeDeleted;
					mainGrid[r2][c].red = mainGrid[r2 - 1][c].red;
					mainGrid[r2][c].green = mainGrid[r2 - 1][c].green;
					mainGrid[r2][c].blue = mainGrid[r2 - 1][c].blue;
				}
			}
			r++;
		}
	}
	deleteLines = false;
}
