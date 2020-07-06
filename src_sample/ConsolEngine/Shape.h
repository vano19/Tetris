#pragma once

#include <vector>
#include <memory>

#define CHAR_EMPTY ' '
#define CHAR_OBJECT 'o' //'◙'
#define CHAR_LOCK '*'
#define CHAR_HASH '#'
#define DEFAULT '`'

#define STEP 1			
#define CSTEP 0.85		
#define START_X 7
#define START_Y 1
#define BG_SIZE_X 17 // size area is 15 + two bordes
#define BG_SIZE_Y 20
#define SHAPE_VALUE 5
#define OFFSET 2
#define SIZE_SCOPE 7

enum class EShape : int
{
	Square = 0,
	Line,
	Arrow,
	Triangle,
	Parallelogram
};
enum class ELetter : int
{
	Left = 75,
	Down = 80,
	Right = 77,
	Space = 32
};

enum EBorde : int
{
	BOTTOM = -1,
	MIDDLE = 0,
	TOP = 1
};
struct Coordinates
{
	int X = {};
	int Y = {};
};

class Shape
{
	
public:
	using Matrix = std::vector<std::vector<char>>;
	Shape(const Coordinates&, int, int);
	virtual ~Shape() = default;

	inline int getSizeX() const { return sizeX; }
	inline int getSizeY() const { return sizeY; }

	virtual std::shared_ptr<Shape> copy() = 0;
	virtual void rotate() = 0;

	Coordinates coord;
	Matrix arr;

protected:
	void flip();
	int sizeX;
	int sizeY;
};

class Square : public Shape
{
public:
	Square(int, int);
	virtual std::shared_ptr<Shape> copy() override;
	virtual void rotate() override {}
};

class Line : public Shape
{
public:
	Line(int, int, bool = false);
	virtual std::shared_ptr<Shape> copy() override;
	virtual void rotate() override;
	bool mySide;
};

class Arrow : public Shape
{
public:
	Arrow(int, int, int = {});
	virtual std::shared_ptr<Shape> copy() override;
	virtual void rotate() override;
	int mySide;
};


class Triangle : public Shape
{
public:
	Triangle(int, int, int = {}, bool = false);
	virtual std::shared_ptr<Shape> copy() override;
	virtual void rotate() override;
	int mySide;
	const bool myFlip;
};

class Parallelogram : public Shape
{
public:
	Parallelogram(int, int, int = {}, bool = true);
	virtual std::shared_ptr<Shape> copy() override;
	virtual void rotate() override;
	int mySide;
	const bool myFlip;
};