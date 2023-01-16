#include "LogoLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineResources.h>
#include "LogoUI.h"
#include "FadeUI.h"
#include "BlackBackUI.h"
#include "ContentFunc.h"

LogoLevel::LogoLevel()
{
}

LogoLevel::~LogoLevel()
{
}

void LogoLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Logo.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Gray.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Black.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeCircle.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeAnim.BMP"))->Cut(1, 35);

	CreateActor<FadeUI>(1);
	CreateActor<LogoUI>(0);
	CreateActor<BlackBackUI>(-1);
}

void LogoLevel::Update(float _DT)
{
	if (true == GameEngineInput::IsAnyKey())
	{
		FadeUI::ActiveFade(FADE_STATE::FADEIN, this, ContentFunc::ChangeTitleLevel);
	}
}

void LogoLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	FadeUI::ActiveFade(FADE_STATE::FADEOUT, this, nullptr);
}
