#include "MapToolLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include "GrayBackUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"

MapToolLevel::MapToolLevel() :
	MapToolFadeActor(nullptr)
{
}

MapToolLevel::~MapToolLevel()
{
}

void MapToolLevel::Loading()
{
	CreateActor<GrayBackUI>();
	MapToolFadeActor = CreateActor<FadeUI>();

	if (false == GameEngineInput::IsKey("MapEscape"))
	{
		GameEngineInput::CreateKey("MapEscape", VK_ESCAPE);
	}
}

void MapToolLevel::Update(float _DT)
{
	if (false == MapToolFadeActor->IsProgress() && GameEngineInput::IsDown("MapEscape"))
	{
		MapToolFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeTitleLevel);
	}
}

void MapToolLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	MapToolFadeActor->Fade(FADE_STATE::FADEOUT);
}

void MapToolLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{

}