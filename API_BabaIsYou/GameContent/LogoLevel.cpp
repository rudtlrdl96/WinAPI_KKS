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

#include "ContentDataLoader.h"

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
	Dir.Move("Logo");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Logo.BMP"))->Cut(1, 3);

	Dir.MoveParent();
	Dir.Move("Defalut");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Gray.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Black.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeCircle.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeAnim.BMP"))->Cut(1, 35);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Text.BMP"))->Cut(37, 1);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WiggleText.BMP"))->Cut(37, 3);

	LogoFadeActor = CreateActor<FadeUI>(1);
	CreateActor<LogoUI>(0);
	CreateActor<BlackBackUI>(-1);
}

void LogoLevel::Update(float _DT)
{
	if (false == LogoFadeActor->IsProgress() && true == GameEngineInput::IsAnyKey())
	{
		LogoFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeTitleLevel);
	}
}

void LogoLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	LogoFadeActor->Fade(FADE_STATE::FADEOUT);
}
