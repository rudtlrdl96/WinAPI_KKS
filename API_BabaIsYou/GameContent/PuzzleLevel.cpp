#include "PuzzleLevel.h"
#include "SubjectManager.h"
#include "RuleManager.h"

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
	if (nullptr == SubjectMgr)
	{
		SubjectMgr = new SubjectManager;
	}
	if (nullptr == RuleMgr)
	{
		RuleMgr = new RuleManager;
	}

	LoadPuzzleData();
}

void PuzzleLevel::Update()
{
	int a = 0;
}

void PuzzleLevel::LoadPuzzleData()
{

}