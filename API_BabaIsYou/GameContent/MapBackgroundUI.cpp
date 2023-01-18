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
	InitRender({
	.FileName = "WorldMap.BMP",
	.Pos = GameEngineWindow::GetScreenSize().half(),
	.Scale = GameEngineWindow::GetScreenSize() - float4{ 90, 70 },
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAP),
	.BitmapInterval = 1,
	.IsUI = true });		
}