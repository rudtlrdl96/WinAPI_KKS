#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>

#include "FadeUI.h"
#include "PuzzleActorManager.h"
#include "RuleManager.h"
#include "GrayBackUI.h"
#include "CongratulationsUI.h"
#include "ContentFunc.h"
#include "ContentConst.h"

std::string PuzzleLevel::LoadPuzzleName = "";
std::string PuzzleLevel::LoadPuzzleInfo = "";
size_t PuzzleLevel::LevelNumber = 0;

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

void PuzzleLevel::SetPuzzleMapInfo(const std::string_view& _MapInfo)
{
	LoadPuzzleInfo = _MapInfo;
}

void PuzzleLevel::SetPuzzleMapLevel(size_t _Level)
{
	LevelNumber = _Level;
}

void PuzzleLevel::Restart()
{
	PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangePuzzleLevel });
}

void PuzzleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("Puzzle");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CongratulationsAnim.BMP"))->Cut(1, 37);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CongratulationsWiggle.BMP"))->Cut(1, 9);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Effect.BMP"))->Cut(1, 1);
		
	CreateActor<GrayBackUI>();
	PuzzleFadeActor = CreateActor<FadeUI>();
	CongratulationActor = CreateActor<CongratulationsUI>();

	PuzzleActorManager::GetInst()->Init(this);

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

}

void PuzzleLevel::Update(float _DT)
{
	if (false == PuzzleFadeActor->IsProgress() && true == IsExitValue)
	{
		PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeWorldmapLevel });
	}

	if (true == IsExitValue)
	{
		return;
	}

	if (true == PuzzleActorManager::GetInst()->IsPuzzleEnd() && false == CongratulationActor->IsProgress())
	{
		CongratulationActor->Congratulations(&PuzzleLevelExit);
	}
	else
	{
		PuzzleActorManager::GetInst()->Input(_DT);
		PuzzleActorManager::GetInst()->clear();
	}
}

void PuzzleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	IsExitValue = false;
	LoadPuzzleData();
	PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEOUT, .WaitTime = 4.0f,
		.ShakeDistance = 7.0f,
		.ShakeTime = 0.95f,
		.WriteTopText = "LEVEL " + std::to_string(LevelNumber),
		.TopTextInterval = {40, 0},
		.TopTextSize = {35, 35},
		.TopTextColor = TEXT_COLOR::PINK,
		.WriteMiddleText = LoadPuzzleInfo,
		.MiddleTextInterval = {40, 0},
		.MiddleTextSize = {80, 80},
		});
}

void PuzzleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	PuzzleActorManager::GetInst()->Reset();
	CongratulationActor->Off();
}

void PuzzleLevel::LoadPuzzleData()
{
	PuzzleActorManager::GetInst()->LoadData(LoadPuzzleName);
}