#include "TitleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>

#include "TitleLogoUI.h"
#include "BlackBoxUI.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{
}

void TitleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TitleLogo.BMP"))->Cut(1, 3);

	CreateActor<TitleLogoUI>();
	CreateActor<BlackBoxUI>();

	if (false == GameEngineInput::IsKey("LevelChange"))
	{
		GameEngineInput::CreateKey("LevelChange", VK_SPACE);
	}
}

void TitleLevel::Update(float _DT)
{
	if (true == GameEngineInput::IsDown("LevelChange"))
	{
		GameEngineCore::GetInst()->ChangeLevel("Worldmap");
	}
}