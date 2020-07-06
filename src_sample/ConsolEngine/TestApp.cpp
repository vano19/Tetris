#include "TestApp.h"
#include <ctime>
#include <iostream>
#include <functional>

TestApp::TestApp() 
	: BaseApp(BG_SIZE_X + SIZE_SCOPE, BG_SIZE_Y + SHAPE_VALUE)
	, oldObj(nullptr)
	, currentTime(NULL)
	, score(NULL)
	, step(STEP)
	, nextObj( static_cast<EShape>(rand() % SHAPE_VALUE ) )
	, nextFlip( rand() % OFFSET)
{
	srand(time(NULL));
	createObj();
}

void TestApp::KeyPressed( ELetter letter )
{
	
	switch (letter)
	{
		case ELetter::Left:
		{
			if (leftIsEmpty()) obj->coord.X--;
			break;
		}
		case ELetter::Down:
		{
			while (downIsEmpty()) 
				obj->coord.Y++;
				
			currentTime = step;
			break;
		}
		case ELetter::Right:
		{
			if (rightIsEmpty()) obj->coord.X++;
			break;
		}
		case ELetter::Space:
		{
			setBy(oldObj, CHAR_EMPTY);
			if (canBeRotate())
				obj->rotate();
			oldObj = obj->copy();
			break;
		}
		default: return;
	};
	
	
	if ( obj->coord.X < 0 ) obj->coord.X = 0;
	else if ( obj->coord.X + obj->getSizeX() > BG_SIZE_X - 1 )
		obj->coord.X = BG_SIZE_X - obj->getSizeX();
}

bool TestApp::downIsEmpty()
{
	if (obj->coord.Y + obj->getSizeY() > BG_SIZE_Y)
		return false;

	for (int x = 0; x < obj->getSizeX(); x++)
	{
		for (int y = obj->getSizeY() - 1; y >= 0; y--)
			if (obj->arr[x][y] != CHAR_EMPTY)
			{
				if (GetChar(obj->coord.X + x, obj->coord.Y + y + 1) != CHAR_EMPTY)
					return false;
				else
					break;
			}
	}
	return true;
}

void TestApp::UpdateF(float deltaTime)
{
	setBy(oldObj, CHAR_EMPTY);
	burn();
	currentTime += deltaTime;
	if (currentTime > step)
	{
		currentTime = 0;
		if (downIsEmpty())
			obj->coord.Y++;
		else
		{
			lockIn();
			if (!createObj())
				gameOver();
		}
	}
	oldObj = obj->copy();
	printNextObj();
	setBy(obj);
	printWalls();
	showInfo();
}

bool TestApp::createObj()
{
	int side = rand() % 4;
	switch (static_cast<EShape>(nextObj))
	{
	case EShape::Square :
		obj = std::make_shared<Square>( START_X, START_Y );
		break;
	case EShape::Line:
		obj = std::make_shared<Line>(START_X, START_Y, (side) ? side - OFFSET : side);
		break;
	case EShape::Arrow :
		obj = std::make_shared<Arrow>(START_X, START_Y, side);
		break;
	case EShape::Triangle:
		obj = std::make_shared<Triangle>(START_X, START_Y, side, nextFlip);
		break;
	case EShape::Parallelogram:
		obj = std::make_shared<Parallelogram>(START_X, START_Y, side, nextFlip);
		break;
	default:
		break;
	}

	nextFlip = rand() % 2;
	nextObj = static_cast<EShape>(rand() % SHAPE_VALUE);

	for (int x = 0; x < obj->getSizeX(); x++)
		for (int y = 0; y < obj->getSizeY(); y++)
			if (GetChar(obj->coord.X + x, obj->coord.Y + y) != CHAR_EMPTY)
				return false;

	oldObj = obj->copy();
	return true;
}

void TestApp::printWalls()
{

	for (int y = 0; y <= Y_SIZE; y++)
	{
		SetChar(0, y, CHAR_HASH);
		SetChar(X_SIZE, y, CHAR_HASH);
		if (y <= BG_SIZE_Y)
			SetChar(BG_SIZE_X, y, CHAR_HASH);
	
	}
	for (int x = 0; x < X_SIZE; x++)
	{
	    SetChar(x, 0, CHAR_HASH);
		SetChar(x, BG_SIZE_Y, CHAR_HASH);
		SetChar(x, Y_SIZE, CHAR_HASH);
	}
}

void TestApp::printNextObj()
{

	for (int y = 1; y <= SHAPE_VALUE+ OFFSET; y++)
		for (int x = BG_SIZE_X-1; x < X_SIZE; x++ )
			SetChar(x, y, CHAR_EMPTY);

	std::shared_ptr<Shape> next = nullptr;
	switch (nextObj)
	{
	case EShape::Square:
		next = std::make_shared<Square>(X_SIZE - 4, OFFSET);
		break;
	case EShape::Line:
		next = std::make_shared<Line>(X_SIZE - SHAPE_VALUE, 3, true);
		break;
	case EShape::Arrow:
		next = std::make_shared<Arrow>(X_SIZE - SHAPE_VALUE, OFFSET, OFFSET);
		break;
	case EShape::Triangle:
		next = std::make_shared<Triangle>(X_SIZE - SHAPE_VALUE, OFFSET, nextFlip ? 1 : 3, nextFlip);
		break;
	case EShape::Parallelogram:
		next = std::make_shared<Parallelogram>(X_SIZE - SHAPE_VALUE, OFFSET, 0, nextFlip);
		break;
	default:
		return;
	}
	setBy(next);
}

void TestApp::showInfo()
{
	//for (int i = 0; i < SHAPE_VALUE; i++)
	//	SetChar(X_SIZE - 6 + i, START_X, info[i]);
	//SetChar(X_SIZE - 1, 7, 's');

	const std::string info = std::to_string(score);
	constexpr char score[] = " > Score:";
	for (int i = 0; i < sizeof(score) + info.size(); i++)
		SetChar(OFFSET + i, BG_SIZE_Y + OFFSET,  ( i < sizeof(score) ? score[i] : info[i - sizeof(score)] ) );

}


void TestApp::lockIn()
{
	setBy(oldObj, CHAR_EMPTY);
	setBy(obj, CHAR_LOCK);
	obj.reset();
	oldObj.reset();
}

void TestApp::setBy( std::shared_ptr<Shape> object, wchar_t setter)
{
	for (int x = 0; x < object->getSizeX(); x++)
		for (int y = 0; y < object->getSizeY(); y++)
			if (object->arr[x][y] != CHAR_EMPTY)
				SetChar(x + object->coord.X, y + object->coord.Y, setter == DEFAULT ? object->arr[x][y] : setter );
}

bool TestApp::canBeRotate()
{
	std::shared_ptr<Shape> temp( obj->copy() );
	temp->rotate();
	for (int y = 0; y < temp->getSizeY(); y++)
		for (int x = 0; x < temp->getSizeX(); x++)
			if (temp->arr[x][y] != CHAR_EMPTY && GetChar(temp->coord.X + x, temp->coord.Y + y) == CHAR_LOCK)
				return false;
	return true;
}

bool TestApp::burn()
{
	bool filledString;
	for (int y = BG_SIZE_Y - 1; y > 0; y--)
	{
		filledString = true;
		for (int x = 1; x < BG_SIZE_X; x++)
		{
			if (GetChar(x, y) == CHAR_EMPTY)
			{
				filledString = false;
				break;
			}
		}
		if (!filledString)
			continue;

		for (int x = 1; x < BG_SIZE_X; x++)
			SetChar(x, y, CHAR_EMPTY);

		for (int i = y; i > 1; i--)
			for (int x = 0; x < BG_SIZE_X; x++)
				SetChar(x, i, GetChar(x, i - 1));

		score += BG_SIZE_X - OFFSET;
		step *= CSTEP;
		return true;
	}
	return false;
}

void TestApp::gameOver()
{
	system("cls");
	std::cout << "Game over!\nYour score: " << score << std::endl;
	std::cin.ignore();
	exit(0);
}

bool TestApp::rightIsEmpty()
{
	for (int y = 0; y < obj->getSizeY(); y++)
		for (int x = obj->getSizeX() - 1; x >= 0; x--)
			if (obj->arr[x][y] != CHAR_EMPTY)
			{
				if (GetChar(obj->coord.X + x + 1, obj->coord.Y + y) == CHAR_LOCK)
					return false;
				else 
					break;
			}

	return true;
}

bool TestApp::leftIsEmpty()
{
	for (int y = 0; y < obj->getSizeY(); y++)
		for (int x = 0; x < obj->getSizeX(); x++)
			if (obj->arr[x][y] != CHAR_EMPTY)
			{
				if (GetChar(obj->coord.X + x - 1, obj->coord.Y + y) != CHAR_EMPTY)
					return false;
				else
					break;
			}

	return true;
}