#include "TitleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineResources.h>

#include "TitleLogoUI.h"
#include "BlackBoxUI.h"

bool TitleLevel::LoadDoubleCheck = false;

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
}

void TitleLevel::Update()
{
}