#include "MapToolLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include "GrayBackUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"
#include "ContentConst.h"
#include "BlackBackUI.h"
#include "PalletActor.h"
#include "ButtonUI.h"
#include "MapGridData.h"

MapToolLevel::MapToolLevel() :
	MapToolFadeActor(nullptr)
{
}

MapToolLevel::~MapToolLevel()
{
}

void MapToolLevel::SetSize(int2 _Size)
{
	if (1 > _Size.x || _Size.x > ContentConst::GRID_SIZE_X)
	{
		return;
	}

	if (1 > _Size.y || _Size.y > ContentConst::GRID_SIZE_Y)
	{
		return;
	}

	MapSize = _Size;

	float4 TotalGridSize = { ContentConst::ACTOR_SIZE.x * MapSize.x, ContentConst::ACTOR_SIZE.y * MapSize.y };
	GridBackActor->SetScale(TotalGridSize);
}

void MapToolLevel::Loading()
{
	CreateActor<GrayBackUI>();
	MapToolFadeActor = CreateActor<FadeUI>();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("MapTool");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Pallet.BMP"))->Cut(3, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("SelectPallet.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CloseButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("ObjectsButton.BMP"))->Cut(1, 2);

	if (false == GameEngineInput::IsKey("MapEscape"))
	{
		GameEngineInput::CreateKey("MapEscape", VK_ESCAPE);
	}

	GridBackActor = CreateActor<BlackBackUI>();
	Pallet = CreateActor<PalletActor>();
	Pallet->DisablePallet();

	ObjectButton = CreateActor<ButtonUI>();
	ObjectButton->SetImage("ObjectsButton.BMP", {160, 40});
	ObjectButton->SetColSize({ 160, 40 });
	ObjectButton->SetPos({ 1180, 690});

	SetSize({10, 10});
}

void MapToolLevel::Update(float _DT)
{
	if (true == Pallet->IsUpdate())
	{
		return;
	}
	
	if (true == ObjectButton->IsUp())
	{
		Pallet->ActivePallet();
	}
	else if (false == MapToolFadeActor->IsProgress() && GameEngineInput::IsDown("MapEscape"))
	{
		MapToolFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeTitleLevel);
	}
	else if (false == MapToolFadeActor->IsProgress() && true == GameEngineInput::IsPress("MouseLeft"))
	{
		
	}
}

void MapToolLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	MapToolFadeActor->Fade(FADE_STATE::FADEOUT);
}

void MapToolLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	
}


void MapToolLevel::DisableMap()
{
	for (size_t y = 0; y < vecMapDatas.size(); y++)
	{
		for (size_t x = 0; x < vecMapDatas[y].size(); x++)
		{
			vecMapDatas[y][x]->Off();
		}
	}
}

void MapToolLevel::ActiveMap()
{
	for (size_t y = 0; y < MapSize.y; y++)
	{
		for (size_t x = 0; x < MapSize.x; x++)
		{
			//vecMapDatas[y][x]->Off();
		}
	}
}

void MapToolLevel::ResizeMap(const int2& _MapSize)
{

}