#include "PuzzleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineResources.h>

#include "SubjectManager.h"
#include "RuleManager.h"
#include "CharacterActor.h"

std::string PuzzleLevel::LoadPuzzleName = "";

PuzzleLevel::PuzzleLevel()
{
}

PuzzleLevel::~PuzzleLevel()
{
	if (nullptr != SubjectMgr)
	{
		delete SubjectMgr;
		SubjectMgr = nullptr;
	}

	if (nullptr != RuleMgr)
	{
		delete RuleMgr;
		RuleMgr = nullptr;
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


	if (nullptr == SubjectMgr)
	{
		SubjectMgr = new SubjectManager;
		SubjectMgr->InitGrid();
	}
	if (nullptr == RuleMgr)
	{
		RuleMgr = new RuleManager;
	}

	LoadPuzzleData();

	CreateActor<CharacterActor>();
}

void PuzzleLevel::Update(float _DT)
{
	int a = 0;
}

void PuzzleLevel::LoadPuzzleData()
{

}