#include "TitleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>

#include "FadeUI.h"
#include "TitleLogoUI.h"
#include "BlackBackUI.h"
#include "ContentFunc.h"
#include "ButtonUI.h"

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
	Dir.Move("Title");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TitleLogo.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("StartButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapToolButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("ExitButton.BMP"))->Cut(1, 2);

	TitleFadeActor = CreateActor<FadeUI>();
	CreateActor<TitleLogoUI>();
	CreateActor<BlackBackUI>();
	{
		GameStartButton = CreateActor<ButtonUI>();
		GameStartButton->SetColSize({ 400, 60 });
		GameStartButton->SetPos({640, 410});
		GameStartButton->SetImage("StartButton.BMP", { 400, 60 });
	}
	{
		MapToolButton = CreateActor<ButtonUI>();
		MapToolButton->SetColSize({ 400, 60 });
		MapToolButton->SetPos({ 640, 480 });
		MapToolButton->SetImage("MapToolButton.BMP", { 400, 60 });
	}
	{
		ExitButton = CreateActor<ButtonUI>();
		ExitButton->SetColSize({ 400, 60 });
		ExitButton->SetPos({ 640, 550 });
		ExitButton->SetImage("ExitButton.BMP", { 400, 60 });
	}

	if (false == GameEngineInput::IsKey("MouseLeft"))
	{
		GameEngineInput::CreateKey("MouseLeft", VK_LBUTTON);
	}

}

void TitleLevel::Update(float _DT)
{
	if (false == TitleFadeActor->IsProgress() && true == GameStartButton->IsUp())
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeWorldmapLevel);
	}
	else if (false == TitleFadeActor->IsProgress() && true == MapToolButton->IsUp())
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeMapToolLevel);
	}
	else if (false == TitleFadeActor->IsProgress() && true == ExitButton->IsUp())
	{
		GameEngineWindow::AppOff();
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	TitleFadeActor->Fade(FADE_STATE::FADEOUT);
}