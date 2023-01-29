#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>

#include "FadeUI.h"
#include "GridActorManager.h"
#include "RuleManager.h"
#include "GrayBackUI.h"
#include "CongratulationsUI.h"
#include "ContentFunc.h"
#include "ContentConst.h"
#include "ContentDataBase.h"

std::string PuzzleLevel::LoadPuzzleName = "babaisyou";
bool PuzzleLevel::IsExitValue = false;

PuzzleLevel::PuzzleLevel()
{
}

PuzzleLevel::~PuzzleLevel()
{
}

void PuzzleLevel::PuzzleLevelExit()
{
	IsExitValue = true;
}

void PuzzleLevel::SetPuzzleMapName(const std::string_view& _MapName)
{
	LoadPuzzleName = _MapName;
}

void PuzzleLevel::Restart()
{
	PuzzleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangePuzzleLevel);
}

void PuzzleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("Puzzle");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("actor.BMP"))->Cut(24, 40);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CongratulationsAnim.BMP"))->Cut(1, 37);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CongratulationsWiggle.BMP"))->Cut(1, 9);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Effect.BMP"))->Cut(1, 1);
		
	CreateActor<GrayBackUI>();
	PuzzleFadeActor = CreateActor<FadeUI>();
	CongratulationActor = CreateActor<CongratulationsUI>();

	GridActorManager::GetInst()->Init(this);

	if (false == GameEngineInput::IsKey("Wait"))
	{
		GameEngineInput::CreateKey("ReStart", 'R');
		GameEngineInput::CreateKey("ArrowUp", VK_UP);
		GameEngineInput::CreateKey("ArrowDown", VK_DOWN);
		GameEngineInput::CreateKey("ArrowLeft", VK_LEFT);
		GameEngineInput::CreateKey("ArrowRight", VK_RIGHT);
		GameEngineInput::CreateKey("Undo", 'Z');
		GameEngineInput::CreateKey("Wait", VK_SPACE);
	}

	ContentDataBase::GetInst()->InitDataBase();
}

void PuzzleLevel::Update(float _DT)
{
	if (false == PuzzleFadeActor->IsProgress() && true == IsExitValue)
	{
		PuzzleFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeWorldmapLevel);
	}

	if (true == IsExitValue)
	{
		return;
	}

	if (true == GridActorManager::GetInst()->IsPuzzleEnd() && false == CongratulationActor->IsProgress())
	{
		CongratulationActor->Congratulations(&PuzzleLevelExit);
	}
	else
	{
		GridActorManager::GetInst()->Input(_DT);
		GridActorManager::GetInst()->clear();
	}
}

void PuzzleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	IsExitValue = false;
	LoadPuzzleData();
	PuzzleFadeActor->Fade(FADE_STATE::FADEOUT);
}

void PuzzleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	GridActorManager::GetInst()->Reset();
	CongratulationActor->Off();
}

void PuzzleLevel::LoadPuzzleData()
{
	GridActorManager::GetInst()->LoadData(LoadPuzzleName);
}