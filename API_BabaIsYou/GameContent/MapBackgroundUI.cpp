#include "MapBackgroundUI.h"

MapBackgroundUI::MapBackgroundUI()
{
}

MapBackgroundUI::~MapBackgroundUI()
{
}

void MapBackgroundUI::Start()
{
	MapBackgroundImage = GameEngineResources::GetInst().ImageFind("WorldMap.BMP");

	SetPos(GameEngineWindow::GetScreenSize().half());
}

void MapBackgroundUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		MapBackgroundImage,
		GetPos(), GameEngineWindow::GetScreenSize() - float4{90, 70},
		{ 0, static_cast<float>(461 * (GetWiggleIndex())) },
		{ 854, 461 });
}