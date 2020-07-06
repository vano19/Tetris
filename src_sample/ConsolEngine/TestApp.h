#pragma once

#include "BaseApp.h"
#include "Shape.h"
#include<memory>
#include <type_traits>
#include <iterator>
#include <algorithm>

class TestApp : public BaseApp
{
public:
	TestApp();
	virtual void KeyPressed(ELetter btnCode) override;
	virtual void UpdateF(float deltaTime) override;
	bool createObj();
	void printWalls();
	void printNextObj();
	void showInfo();
	void lockIn();
	bool canBeRotate();
	bool burn();
	void gameOver();
	void setBy( std::shared_ptr<Shape>, wchar_t s = DEFAULT);
	bool downIsEmpty();
	bool leftIsEmpty();
	bool rightIsEmpty();

private:
	int score;
	float currentTime, step;
	std::shared_ptr<Shape> obj, oldObj;
	EShape nextObj;
	bool nextFlip;
};




//, typename R = std::enable_if< std::is_same<Begin, End>::value, bool >::type  >
