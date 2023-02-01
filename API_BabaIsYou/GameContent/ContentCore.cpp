#include "ContentCore.h"

#include <GameEnginePlatform/GameEngineWindow.h>

#include "TitleLevel.h"
#include "MapToolLevel.h"
#include "WorldMapLevel.h"
#include "PuzzleLevel.h"
#include "EndingLevel.h"
#include "LogoLevel.h"

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
	CreateLevel<PuzzleLevel>("Puzzle");
	CreateLevel<WorldMapLevel>("Worldmap");
	CreateLevel<EndingLevel>("Ending");
	CreateLevel<MapToolLevel>("MapTool");

	ChangeLevel("Logo");
}

void ContentCore::Update()
{

}

void ContentCore::End()
{
}