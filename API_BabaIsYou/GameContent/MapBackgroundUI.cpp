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
}

void MapBackgroundUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		MapBackgroundImage,
		float4{45, 35}, GameEngineWindow::GetScreenSize() - float4{ 90, 70 },
		{ 0, static_cast<float>(461 * (GetWiggleIndex())) },
		{ 854, 461 });
}