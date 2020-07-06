#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "Shape.h"

#define BUFF_SIZE 255
#define COUNT 1000

class BaseApp
{
public:
	const int X_SIZE;
	const int Y_SIZE;


	BaseApp(int xSize = 100, int ySize = 80);
	virtual ~BaseApp() = default;

	void Run();

	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	virtual void UpdateF(float deltaTime) = 0;
	virtual void KeyPressed(ELetter btnCode) = 0;


private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	std::unique_ptr<CHAR_INFO[]> mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;

	void Render();
};