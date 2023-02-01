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
#include "WiggleMapToolActor.h"

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

	vecTitleButtons.reserve(TB_COUNT);

	for (size_t i = 0; i < vecTitleButtons.capacity(); i++)
	{
		vecTitleButtons.push_back(CreateActor<ButtonUI>());
	}

	{
		vecTitleButtons[TB_GAME]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_GAME]->SetPos({640, 410});
		vecTitleButtons[TB_GAME]->SetImage("StartButton.BMP", { 400, 60 });
	}
	{
		vecTitleButtons[TB_MAPTOOL]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_MAPTOOL]->SetPos({ 640, 480 });
		vecTitleButtons[TB_MAPTOOL]->SetImage("MapToolButton.BMP", { 400, 60 });
	}
	{
		vecTitleButtons[TB_EXIT]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_EXIT]->SetPos({ 640, 550 });
		vecTitleButtons[TB_EXIT]->SetImage("ExitButton.BMP", { 400, 60 });
	}

	ButtonBaba = CreateActor<WiggleMapToolActor>();
	ButtonBaba->SetRender("BABA", DIR_FLAG::RIGHT);

	if (false == GameEngineInput::IsKey("MouseLeft"))
	{
		GameEngineInput::CreateKey("MouseLeft", VK_LBUTTON);
		GameEngineInput::CreateKey("ButtonUp", VK_UP);
		GameEngineInput::CreateKey("ButtonDown", VK_DOWN);
		GameEngineInput::CreateKey("ButtonUse", VK_SPACE);
	}

	BaBaSetPos();
}

void TitleLevel::Update(float _DT)
{
	if (nullptr == GetFocus())
	{
		return;
	}

	if (true == TitleFadeActor->IsProgress())
	{
		return;
	}

	if (true == GameEngineInput::IsDown("ButtonUp"))
	{
		ButtonUp();
	}
	else if (true == GameEngineInput::IsDown("ButtonDown"))
	{
		ButtonDown();
	}
	else if (true == GameEngineInput::IsDown("ButtonUse"))
	{
		ButtonUse();
	}

	if (true == vecTitleButtons[TB_GAME]->IsUp())
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeWorldmapLevel);
	}
	else if (true == vecTitleButtons[TB_MAPTOOL]->IsUp())
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeMapToolLevel);
	}
	else if (true == vecTitleButtons[TB_EXIT]->IsUp())
	{
		GameEngineWindow::AppOff();
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	TitleFadeActor->Fade(FADE_STATE::FADEOUT);
}

void TitleLevel::ButtonUp()
{
	if (0 >= SelectButton)
	{
		return;
	}

	--SelectButton;
	BaBaSetPos();
}

void TitleLevel::ButtonDown()
{
	if (vecTitleButtons.size() - 1 <= SelectButton)
	{
		return;
	}

	++SelectButton;
	BaBaSetPos();
}

void TitleLevel::ButtonUse()
{
	if (TB_GAME == SelectButton)
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeWorldmapLevel);
	}
	else if (TB_MAPTOOL == SelectButton)
	{
		TitleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeMapToolLevel);
	}
	else if (TB_EXIT == SelectButton)
	{
		GameEngineWindow::AppOff();
	}
}

void TitleLevel::BaBaSetPos()
{
	ButtonBaba->SetPos(vecTitleButtons[SelectButton]->GetPos() - float4{230.0f, 0.0f});
}