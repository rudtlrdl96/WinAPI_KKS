#include "PuzzleLevel.h"
#include <GameEngineBase/GameEnginePath.h>
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
#include "TutorialUI.h"
#include "ContentRand.h"
#include "AppearParticle.h"
#include "CameraSystem.h"
#include "SoundSystem.h"

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
	IsReStart = true;
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
	
	GameEngineDirectory SoundDir;

	SoundDir.MoveParentToDirectory("ContentsResources");
	SoundDir.Move("ContentsResources");
	SoundDir.Move("Sound");
	SoundDir.Move("BGM");

	SoundSystemPtr = CreateActor<SoundSystem>();
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("baba.ogg"), SOUND_GROUP::BGM);
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("noise.ogg"), SOUND_GROUP::BGM);

	SoundDir.MoveParent();
	SoundDir.Move("Effect");

	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("Move_"), ".ogg", SOUND_GROUP::EFFECT, 17);
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("Undo_"), ".ogg", SOUND_GROUP::EFFECT, 5);
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("Defeat_"), ".ogg", SOUND_GROUP::EFFECT, 4);
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("Sink_"), ".ogg", SOUND_GROUP::EFFECT, 4);
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("Melt_"), ".ogg", SOUND_GROUP::EFFECT, 5);
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("TextCompletion_"), ".ogg", SOUND_GROUP::EFFECT, 5);
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("Win.ogg"), SOUND_GROUP::EFFECT);
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("Restart.ogg"), SOUND_GROUP::EFFECT);

	CreateActor<GrayBackUI>();
	PuzzleFadeActor = CreateActor<FadeUI>();
	CongratulationActor = CreateActor<CongratulationsUI>();
	TutorialRender = CreateActor<TutorialUI>();

	PuzzleActorManager::GetInst()->Init(this);

	if (false == GameEngineInput::IsKey("Wait"))
	{
		GameEngineInput::CreateKey("ReturnWorldmap", VK_DELETE);
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
	if (false == PuzzleFadeActor->IsProgress() && true == GameEngineInput::IsUp("ReturnWorldmap"))
	{
		PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeWorldmapLevel });
		return;
	}

	if (false == PuzzleFadeActor->IsProgress() && true == IsExitValue)
	{
		PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeWorldmapLevel });
		SoundSystemPtr->BgmStop();
	}

	if (true == IsExitValue)
	{
		return;
	}

	if (true == PuzzleActorManager::GetInst()->IsPuzzleEnd() && false == CongratulationActor->IsProgress())
	{
		CongratulationActor->Congratulations(&PuzzleLevelExit);
		SoundSystemPtr->Play("Win.ogg");
	}
	else
	{
		if (false == PuzzleFadeActor->IsProgress())
		{
			PuzzleActorManager::GetInst()->Input(_DT);
			PuzzleActorManager::GetInst()->clear();
		}	
	}
}

void PuzzleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	IsExitValue = false;
	LoadPuzzleData();

	if (true == IsReStart)
	{
		PuzzleFadeActor->Fade({ .State = FADE_STATE::FADEOUT, .WaitTime = 0.5f});
		IsReStart = false;
	}
	else
	{
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