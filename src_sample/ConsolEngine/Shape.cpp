#include "Shape.h"

Shape::Shape(const Coordinates& c, int arrX, int arrY) :
	sizeX(arrX),
	sizeY(arrY),
	arr{arrX, std::vector<char>(arrY, CHAR_EMPTY) }
{
	this->coord = c;
}



void Shape::rotate()
{
	if (arr.empty())
		return;

	std::swap(sizeX, sizeY);
	Matrix temp( sizeX, std::vector<char>(sizeY) );

	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			temp[x][y] = arr[y][sizeX - x - 1];

	arr = std::move(temp);

}

void Shape::flip()
{

	for (int y = 0; y < sizeY; y++)
		for (int x = 0; x < sizeX / 2; x++)
			std::swap(arr[x][y], arr[sizeX - x - 1][y]);

}

Square::Square(int x, int y) :
	Shape(Coordinates{ x, y }, 2, 2)
{
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			arr[x][y] = CHAR_OBJECT;
}

std::shared_ptr<Shape> Square::copy()
{
	return std::make_shared<Square>(coord.X, coord.Y);
}

Line::Line(int x, int y, bool side) :
	Shape(Coordinates{x, y}, 1, 4),
	mySide(0)
{
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			arr[x][y] = CHAR_OBJECT;

	if (side != mySide)
		rotate();
}

std::shared_ptr<Shape> Line::copy()
{
	return std::make_shared<Line>( coord.X, coord.Y, mySide);
}

void Line::rotate()
{
	Shape::rotate();
	mySide = !mySide;
}


Arrow::Arrow(int x, int y, int side) :
	Shape(Coordinates{x, y}, 3, 2),
	mySide(0)
{
	arr[1][1] = CHAR_OBJECT;
	arr[0][0] = CHAR_OBJECT;
	arr[1][0] = CHAR_OBJECT;
	arr[2][0] = CHAR_OBJECT;

	while (mySide < side)
		rotate();
}

std::shared_ptr<Shape> Arrow::copy()
{
	return std::make_shared<Arrow>( coord.X, coord.Y, mySide);
}

void Arrow::rotate()
{
	Shape::rotate();
	if (++mySide > 3)
		mySide = 0;
}

Triangle::Triangle(int x, int y, int side, bool flip) 
	: Shape(Coordinates{x, y}, 2, 3)
	, mySide(0)
	, myFlip(flip)
{
	arr[0][0] = CHAR_OBJECT;
	arr[0][1] = CHAR_OBJECT;
	arr[0][2] = CHAR_OBJECT;
	arr[1][2] = CHAR_OBJECT;

	if (myFlip)Shape::flip();

	while (mySide < side)
		rotate();
}

std::shared_ptr<Shape> Triangle::copy()
{
	return std::make_shared<Triangle>( coord.X, coord.Y, mySide, myFlip);
}

void Triangle::rotate()
{
	Shape::rotate();
	if (++mySide > 3)
		mySide = 0;
}

Parallelogram::Parallelogram(int x, int y, int side, bool flip) :
	Shape(Coordinates{x, y}, 3, 2),
	mySide(0),
	myFlip(flip)
{
	arr[0][0] = CHAR_OBJECT;
	arr[1][0] = CHAR_OBJECT;
	arr[1][1] = CHAR_OBJECT;
	arr[2][1] = CHAR_OBJECT;
	
	if (myFlip) Shape::flip();

	while (mySide < side)
		rotate();
}

std::shared_ptr<Shape> Parallelogram::copy()
{
	return std::make_shared<Parallelogram>( coord.X, coord.Y, mySide, myFlip);
}

void Parallelogram::rotate()
{
	Shape::rotate();
	if (++mySide > 3)
		mySide = 0;
}