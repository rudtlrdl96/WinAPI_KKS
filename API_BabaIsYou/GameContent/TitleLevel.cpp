#include "TitleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>

#include "FadeUI.h"
#include "TitleLogoUI.h"
#include "BlackBackUI.h"
#include "ContentFunc.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{
}

void ChangeLevel()
{

}

void TitleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TitleLogo.BMP"))->Cut(1, 3);

	TitleFadeActor = CreateActor<FadeUI>();
	CreateActor<TitleLogoUI>();
	CreateActor<BlackBackUI>();

	if (false == GameEngineInput::IsKey("MapToolKey"))
	{
		GameEngineInput::CreateKey("MapToolKey", 'm');
		GameEngineInput::CreateKey("GameKey", VK_SPACE);
	}

}

void TitleLevel::Update(float _DT)
{
	if (false == TitleFadeActor->IsProgress() && true == GameEngineInput::IsDown("MapToolKey"))
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeMapToolLevel);
	}
	else if (false == TitleFadeActor->IsProgress() && true == GameEngineInput::IsDown("GameKey"))
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeWorldmapLevel);
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	TitleFadeActor->Fade(FADE_STATE::FADEOUT);
}