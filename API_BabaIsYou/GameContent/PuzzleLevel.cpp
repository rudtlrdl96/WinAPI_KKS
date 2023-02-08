#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>

#include "FadeUI.h"
#include "PuzzleActorManager.h"
#include "RuleManager.h"
#include "GrayBackUI.h"
#include "CongratulationsUI.h"
#include "ContentFunc.h"
#include "ContentConst.h"
#include "ParticleSystem.h"
#include "CameraSystem.h"
#include "TutorialUI.h"
#include "ContentRand.h"
#include "AppearParticle.h"

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
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MoveTutorialUI.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("PauseTutorialUI.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("UndoTutorialUI.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DeathUndoUI.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DeathRestartUI.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FlashedParticle.BMP"))->Cut(2, 1);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WiggleDot.BMP"))->Cut(3, 1);
		
	CreateActor<GrayBackUI>();
	PuzzleFadeActor = CreateActor<FadeUI>();
	CongratulationActor = CreateActor<CongratulationsUI>();
	TutorialRender = CreateActor<TutorialUI>();

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

	CreateActor<ParticleSystem>();
	CameraSystemPtr = CreateActor<CameraSystem>();
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
	if ("Stage_0_0" == LoadPuzzleName)
	{
		TutorialRender->SetTutorialImage("MoveTutorialUI.BMP", {90, 90 });
		TutorialRender->SetPos(GameEngineWindow::GetScreenSize().half() + float4{-22, -230});
	}
	else if ("Stage_0_1" == LoadPuzzleName)
	{
		TutorialRender->SetTutorialImage("PauseTutorialUI.BMP", { 90, 90 });
		TutorialRender->SetPos({200, 120});
	}
	else if ("Stage_0_3" == LoadPuzzleName)
	{
		TutorialRender->SetTutorialImage("UndoTutorialUI.BMP", { 70, 70 });
		TutorialRender->SetPos(GameEngineWindow::GetScreenSize().half() + float4{50, -270});
	}
	else
	{
		TutorialRender->Off();
	}

	PuzzleActorManager::GetInst()->LoadData(LoadPuzzleName);
}