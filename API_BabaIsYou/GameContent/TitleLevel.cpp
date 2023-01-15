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

	CreateActor<FadeUI>();
	CreateActor<TitleLogoUI>();
	CreateActor<BlackBackUI>();

	if (false == GameEngineInput::IsKey("LevelChange"))
	{
		GameEngineInput::CreateKey("LevelChange", VK_SPACE);
	}
}

void TitleLevel::Update(float _DT)
{
	if (true == GameEngineInput::IsDown("LevelChange"))
	{
		FadeUI::FadeIn(this, ContentFunc::ChangeWorldmapLevel);
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	FadeUI::FadeOut(this, nullptr);
}