#include "ContentCore.h"
#include "TitleLevel.h"
#include "WorldMapLevel.h"
#include "PuzzleLevel.h"
#include "EndingLevel.h"
#include "LogoLevel.h"

#include "WiggleManager.h"

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

	CreateLevel<LogoLevel>("Logo");
	CreateLevel<TitleLevel>("Title");
	CreateLevel<WorldMapLevel>("Worldmap");
	CreateLevel<PuzzleLevel>("Puzzle");
	CreateLevel<EndingLevel>("Ending");

	ChangeLevel("Puzzle");
}

void ContentCore::Update()
{
	WiggleManager::Update();
}

void ContentCore::End()
{
}