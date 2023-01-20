#include "WorldMapLevel.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>
#include "MapBackgroundUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"

WorldMapLevel::WorldMapLevel()
{
}

WorldMapLevel::~WorldMapLevel()
{
}

void WorldMapLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMap.BMP"))->Cut(1, 3);

	WorldMapFadeActor = CreateActor<FadeUI>();
	CreateActor<MapBackgroundUI>();

	if (false == GameEngineInput::IsKey("LevelChange"))
	{
		GameEngineInput::CreateKey("LevelChange", VK_SPACE);
	}
}

void WorldMapLevel::Update(float _DT)
{
	if (false == WorldMapFadeActor->IsProgress() && true == GameEngineInput::IsAnyKey())
	{
		WorldMapFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangePuzzleLevel);
	}
}


void WorldMapLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	WorldMapFadeActor->Fade(FADE_STATE::FADEOUT);
}