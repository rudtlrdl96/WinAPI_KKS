#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>

#include "FadeUI.h"
#include "ActorManager.h"
#include "GrayBackUI.h"

std::string PuzzleLevel::LoadPuzzleName = "";

PuzzleLevel::PuzzleLevel()
{
}

PuzzleLevel::~PuzzleLevel()
{
	if (nullptr != ActorMgr)
	{
		delete ActorMgr;
		ActorMgr = nullptr;
	}
}

void PuzzleLevel::SetPuzzleMapName(const std::string_view& _MapName)
{
	LoadPuzzleName = _MapName;
}

void PuzzleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("actor.BMP"))->Cut(24, 40);

	CreateActor<FadeUI>();
	CreateActor<GrayBackUI>();

	if (nullptr == ActorMgr)
	{
		ActorMgr = new ActorManager(this);
	}


	if (false == GameEngineInput::IsKey("ArrowUp"))
	{
		GameEngineInput::CreateKey("ArrowUp", VK_UP);
	}
	if (false == GameEngineInput::IsKey("ArrowDown"))
	{
		GameEngineInput::CreateKey("ArrowDown", VK_DOWN);
	}
	if (false == GameEngineInput::IsKey("ArrowLeft"))
	{
		GameEngineInput::CreateKey("ArrowLeft", VK_LEFT);
	}
	if (false == GameEngineInput::IsKey("ArrowRight"))
	{
		GameEngineInput::CreateKey("ArrowRight", VK_RIGHT);
	}
	if (false == GameEngineInput::IsKey("Undo"))
	{
		GameEngineInput::CreateKey("Undo", 'Z');
	}
	if (false == GameEngineInput::IsKey("Wait"))
	{
		GameEngineInput::CreateKey("Wait", VK_SPACE);
	}
}

void PuzzleLevel::Update(float _DT)
{
	ActorMgr->Input(_DT);
	ActorMgr->clear();
}

void PuzzleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	LoadPuzzleData();
	FadeUI::ActiveFade(FADE_STATE::FADEOUT, this, nullptr);
}

void PuzzleLevel::LoadPuzzleData()
{
	if (nullptr == ActorMgr)
	{
		MsgAssert("ActorManager가 초기화 되지 않았습니다");
		return;
	}

	ActorMgr->LoadData(LoadPuzzleName);
}