#include "Piece.h"

Piece::Piece() {
	Piece(0);
}

/*
Tạo một khối mới, thiết lập loại, màu sắc và trạng thái xoay ban đầu
*/
Piece::Piece(int numType) {
	type = numType;
	switch (type) {
	case 0: color(1.0, 1.0, 0.0); break; // Màu vàng
	case 1: color(0.5, 0.5, 0.5); break; // Màu xám
	case 2: color(0.0, 1.0, 1.0); break; // Màu xanh cyan
	case 3: color(0.0, 0.0, 1.0); break; // Màu xanh dương
	case 4: color(1.0, 0.0, 0.0); break; // Màu đỏ
	case 5: color(1.0, 0.0, 1.0); break; // Màu tím
	case 6: color(0.0, 0.8, 0.0); break; // Màu xanh lá cây
	}
	rotation = 0;
}

/*
Lấy một mảng chứa thông tin về một khối cụ thể dựa trên trạng thái xoay của nó
*/
const int* Piece::rotations() {
	return gamePieces[type][rotation];
}

/*
Thiết lập màu sắc cho một khối
*/
void Piece::color(float r, float g, float b) {
	redVal = r;
	greenVal = g;
	blueVal = b;
}

/*
Tăng hoặc giảm chỉ số xoay của khối trò chơi
*/
void Piece::rotatePiece(int dir) {
	if (dir > 0) { // Xoay thuận chiều kim đồng hồ
		if (rotation == 3)
			rotation = 0; // Quay lại trạng thái đầu tiên
		else
			rotation += dir;
	}
	else { // Xoay ngược chiều kim đồng hồ
		if (rotation == 0)
			rotation = 3; // Chuyển sang trạng thái xoay cuối cùng
		else
			rotation += dir;
	}
}
