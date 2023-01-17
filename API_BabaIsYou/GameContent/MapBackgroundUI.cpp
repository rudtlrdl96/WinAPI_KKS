#include "MapBackgroundUI.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "ContentEnum.h"

MapBackgroundUI::MapBackgroundUI()
{
}

MapBackgroundUI::~MapBackgroundUI()
{
}

void MapBackgroundUI::Start()
{
	InitRender("WorldMap.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize() - float4{ 90, 70 },
		0, 1, static_cast<int>(RENDER_ORDER::WORLDMAP), 1);
}