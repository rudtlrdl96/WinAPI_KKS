#include "LogoLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineResources.h>
#include "LogoUI.h"
#include "FadeUI.h"
#include "BlackBoxUI.h"

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

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Logo.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Fade.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Fade_Black.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeCircle.BMP"));

	CreateActor<FadeUI>(1);
	CreateActor<LogoUI>();
	CreateActor<BlackBoxUI>(-1);

	FadeUI::FadeIn();
}

void LogoLevel::Update()
{

}