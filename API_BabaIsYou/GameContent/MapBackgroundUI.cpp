#include "MapBackgroundUI.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEnginePlatform/GameEngineWindow.h>

MapBackgroundUI::MapBackgroundUI()
{
}

MapBackgroundUI::~MapBackgroundUI()
{
}

void MapBackgroundUI::Start()
{
	{
		GameEngineRender* Render = CreateRender("WorldMap.BMP", 0);
	
		Render->SetPosition(GameEngineWindow::GetScreenSize().half());
		Render->SetScale(GameEngineWindow::GetScreenSize() - float4{ 90, 70 });
	}
}