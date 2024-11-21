#include "Square.h"
#include "Piece.h"

const int ROWS = 20; // Số hàng
const int COLS = 10; // Số cột

// Lớp chính của trò chơi
class Game {
public:

	// Hàm khởi tạo
	Game();

	// Các phương thức chính của trò chơi
	bool canRotate(Piece p); // Kiểm tra khả năng xoay khối
	bool moveCollision(int dir); // Kiểm tra va chạm khi di chuyển
	bool rotationCollision(); // Kiểm tra va chạm khi xoay
	void clearMainGrid(); // Xóa lưới chính
	void clearNextPieceGrid(); // Xóa lưới hiển thị khối tiếp theo
	void genNextPiece(); // Tạo khối tiếp theo
	void restart(); // Khởi động lại trò chơi
	void move(int dir); // Di chuyển khối
	
	void rotateShape(int rot); // Xoay khối
	void updateActivePiece(); // Cập nhật khối hiện tại
	void updateNextPieceGrid(); // Cập nhật lưới hiển thị khối tiếp theo
	void fixActivePiece(); // Cố định khối hiện tại
	void update(); // Cập nhật trạng thái trò chơi
	void updateActiveAfterCollision(); // Cập nhật khối hiện tại sau va chạm
	void checkLine(); // Kiểm tra dòng đầy
	void clearLine(); // Xóa dòng đầy
	//bool gameOver(); // Kiểm tra kết thúc trò chơi

	// Các khối trong trò chơi
	Piece activePiece; // Khối hiện tại
	Piece nextPiece; // Khối tiếp theo
	Piece activePieceCopy; // Bản sao khối hiện tại

	// Lưới hiện tại của trò chơi
	Square mainGrid[ROWS][COLS];

	// Lưới hiển thị khối tiếp theo
	Square nextPieceGrid[5][5];

	// Dữ liệu trò chơi
	bool killed; // Trạng thái bị mất
	bool paused; // Trạng thái tạm dừng
	bool deleteLines; // Trạng thái xóa dòng
	int linesCleared; // Số dòng đã xóa
	int shapesCount; // Số khối đã tạo
	int timer; // Bộ đếm thời gian
};
