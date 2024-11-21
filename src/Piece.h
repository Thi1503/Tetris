// Các hằng số cơ bản
const int numPieces = 7;	// Số lượng các khối khác nhau
const int numRotations = 4; // Số lần xoay cho mỗi khối
const int numSpaces = 8;	// Dung lượng lưu trữ thông tin về mỗi khối

// Lớp khối trò chơi
class Piece {

public:

	// Các hàm khởi tạo
	Piece();
	Piece(int newPiece);

	// Tọa độ của khối (được tính từ góc trên bên trái của khối, ngay cả khi góc này trống)
	int x;
	int y;

	// Giá trị màu sắc
	float redVal, greenVal, blueVal;

	// Loại khối và trạng thái xoay
	int type;
	int rotation;

	// Xoay khối
	void rotatePiece(int dir);

	// Thiết lập màu sắc khối theo định dạng RGB
	void color(float r, float g, float b);

	// Giá trị để xoay khối
	const int* rotations();
};

// Tất cả các khối với từng trạng thái xoay của chúng trong lưới, theo định dạng {x0, y0, x1, y1, x2, y2, x3, y3}
const int gamePieces[numPieces][numRotations][numSpaces] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1}, // Khối vuông
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3}, // Khối dọc
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2}, // Khối chữ T
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2},
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2}, // Khối chữ L
		{0, 0, 0, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2}, // Khối chữ L ngược
		{0, 0, 1, 0, 2, 0, 0, 1},
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2}, // Khối chữ Z
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2}, // Khối chữ Z ngược
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};
