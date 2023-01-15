#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
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

}

void PuzzleLevel::Update(float _DT)
{
	int a = 0;
}

void PuzzleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	LoadPuzzleData();
	FadeUI::FadeOut(this, nullptr);
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