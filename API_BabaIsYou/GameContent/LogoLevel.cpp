#include "LogoLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineResources.h>
#include "LogoUI.h"
#include "FadeUI.h"
#include "BlackBoxUI.h"

void NextScene()
{
	GameEngineCore::GetInst()->ChangeLevel("Title");
}

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
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Fade.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeAnim.BMP"))->Cut(1, 10);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Fade_Black.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeCircle.BMP"));

	CreateActor<FadeUI>(1);
	CreateActor<LogoUI>(0);
	CreateActor<BlackBoxUI>(-1);

	if (false == GameEngineInput::IsKey("LevelChange"))
	{
		GameEngineInput::CreateKey("LevelChange", VK_SPACE);
	}

	FadeUI::FadeOut(this, nullptr);
}

void LogoLevel::Update(float _DT)
{
	if (true == GameEngineInput::IsDown("LevelChange"))
	{
		FadeUI::FadeIn(this, NextScene);
	}
}
