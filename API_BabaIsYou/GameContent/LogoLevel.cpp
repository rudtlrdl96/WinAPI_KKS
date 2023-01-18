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

	if (false == GameEngineInput::IsKey("DebugCameraLeft"))
	{
		GameEngineInput::CreateKey("DebugCameraUp", 'w');
		GameEngineInput::CreateKey("DebugCameraRight", 'd');
		GameEngineInput::CreateKey("DebugCameraDown", 's');
		GameEngineInput::CreateKey("DebugCameraLeft", 'a');
	}

}

void LogoLevel::Update(float _DT)
{

	if (true == GameEngineInput::IsPress("DebugCameraUp"))
	{
		SetCameraMove(float4::Up * _DT * 100.0f);
	}
	if (true == GameEngineInput::IsPress("DebugCameraRight"))
	{
		SetCameraMove(float4::Right * _DT * 100.0f);
	}
	if (true == GameEngineInput::IsPress("DebugCameraDown"))
	{
		SetCameraMove(float4::Down * _DT * 100.0f);
	}
	if (true == GameEngineInput::IsPress("DebugCameraLeft"))
	{
		SetCameraMove(float4::Left * _DT * 100.0f);
	}

	if (true == GameEngineInput::IsAnyKey())
	{
		FadeUI::ActiveFade(FADE_STATE::FADEIN, this, ContentFunc::ChangeTitleLevel);
	}
}

void LogoLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	FadeUI::ActiveFade(FADE_STATE::FADEOUT, this, nullptr);
}
