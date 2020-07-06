// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include <algorithm>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include <strsafe.h>

#define MY_PERFORMENCE_COUNTER

#include "PerformanceCounter.h"

BaseApp::BaseApp(int xSize, int ySize) 
	: X_SIZE(xSize) 
	, Y_SIZE(ySize)
	, mDwBufferSize{ static_cast<short>(xSize + 1), static_cast<short>(ySize + 1) }
	, mDwBufferCoord{ 0,0 }
	, mLpWriteRegion{ 0,0,static_cast<short>(xSize + 1), static_cast<short>(ySize + 1) }
{
	SMALL_RECT windowSize = { 0, 0, X_SIZE, Y_SIZE };
	COORD windowBufSize = { X_SIZE + 1, Y_SIZE + 1 };

	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	if (!SetConsoleScreenBufferSize(mConsole, windowBufSize))
	{
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
	}
	if (!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
	}

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo = { 1, FALSE };
	SetConsoleCursorInfo(mConsole, &cursorInfo);

	mChiBuffer = std::make_unique<CHAR_INFO[]>( (X_SIZE + 1)*(Y_SIZE + 1) * sizeof(CHAR_INFO) );			

	for (int x = 0; x < X_SIZE + 1; x++)
	{
		for (int y = 0; y < Y_SIZE + 1; y++)
		{
			SetChar(x, y, L' ');
		}
	}
}


void BaseApp::SetChar(int x, int y, wchar_t c)
{
	mChiBuffer[x + (X_SIZE + 1)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE + 1)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;

}

wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE + 1)*y].Char.AsciiChar;
}

void BaseApp::Render()
{
	if (!WriteConsoleOutput(mConsole, mChiBuffer.get(), mDwBufferSize, mDwBufferCoord, &mLpWriteRegion))
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
	}
}

void BaseApp::Run()
{
	CStopwatch timer;
	int sum = 0 , counter = 0, deltaTime = 0;

	while ( true )
	{
		timer.Start();
		if (_kbhit())
		{
			KeyPressed(static_cast<ELetter>(_getch()));
			if (!FlushConsoleInputBuffer(mConsoleIn))
				std::cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}

		UpdateF( deltaTime / 1000.0f );
		Render();
		Sleep(true);

		while (true)
		{
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}

		sum += deltaTime;
		counter++;
		if ( sum >= COUNT )
		{
			TCHAR  szbuff[BUFF_SIZE];
			StringCchPrintf(szbuff, BUFF_SIZE, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);
			counter = sum = 0;
		}
	}
}