#include "ContentCore.h"
#include "TitleLevel.h"
#include "WorldMapLevel.h"
#include "PuzzleLevel.h"
#include "EndingLevel.h"

#include <GameEnginePlatform/GameEngineWindow.h>

ContentCore::ContentCore()
{
}

ContentCore::~ContentCore()
{
}

void ContentCore::Start()
{
	GameEngineWindow::SettingWindowSize({1280, 720});

	CreateLevel<TitleLevel>("Title");
	CreateLevel<WorldMapLevel>("Worldmap");
	CreateLevel<PuzzleLevel>("Puzzle");
	CreateLevel<EndingLevel>("Ending");

	ChangeLevel("Title");
}

void ContentCore::Update()
{
}

void ContentCore::End()
{
}