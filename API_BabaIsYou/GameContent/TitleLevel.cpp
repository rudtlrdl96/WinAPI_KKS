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

	if (false == GameEngineInput::IsKey("DebugCameraLeft"))
	{
		GameEngineInput::CreateKey("DebugCameraUp", 'w');
		GameEngineInput::CreateKey("DebugCameraRight", 'd');
		GameEngineInput::CreateKey("DebugCameraDown", 's');
		GameEngineInput::CreateKey("DebugCameraLeft", 'a');
	}

}

void TitleLevel::Update(float _DT)
{
	if (true == GameEngineInput::IsDown("LevelChange"))
	{
		FadeUI::ActiveFade(FADE_STATE::FADEIN, this, ContentFunc::ChangeWorldmapLevel);
	}

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
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	FadeUI::ActiveFade(FADE_STATE::FADEOUT, this, nullptr);
}