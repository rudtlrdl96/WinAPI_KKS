#include "PuzzleActorManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentConst.h"
#include "PuzzleActor.h"
#include "CongratulationsUI.h"
#include "BlackBackUI.h"
#include "PuzzleLevel.h"
#include "ContentDataLoader.h"

PuzzleActorManager::PuzzleActorManager()
{

}

PuzzleActorManager::~PuzzleActorManager()
{
}

void PuzzleActorManager::Init(PuzzleLevel* _PuzzleLevel)
{
	MainPuzzleLevel = _PuzzleLevel;
	PuzzleActor::InitGridActor(_PuzzleLevel);
	GridBackActor = MainPuzzleLevel->CreateActor<BlackBackUI>();
}

void PuzzleActorManager::Input(float _DT)
{
	if (true == PuzzleActor::IsWin())
	{
		return;
	}
	PuzzleActor::GetActors(ACTOR_DEFINE::YOU);
	int2 MoveDir = int2::Zero;

	if (GameEngineInput::IsDown("ReStart"))
	{
		MainPuzzleLevel->Restart();
		return;
	}
	else if (GameEngineInput::IsDown("ArrowUp"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::MOVE_UP);
	}
	else if (GameEngineInput::IsDown("ArrowDown"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::MOVE_DOWN);
	}
	else if (GameEngineInput::IsDown("ArrowLeft"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::MOVE_LEFT);
	}
	else if (GameEngineInput::IsDown("ArrowRight"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::MOVE_RIGHT);
	}
	else if (GameEngineInput::IsDown("Undo"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::UNDO);
	}
	else if (GameEngineInput::IsDown("Wait"))
	{
		listInputBuffer.push_back(INPUTBEHAVIOR::WAIT);
	}
	
	if (false == PuzzleActor::IsAnyMove() && 0 < listInputBuffer.size())
	{
		INPUTBEHAVIOR NextBehavior = listInputBuffer.front();
		listInputBuffer.pop_front();

		switch (NextBehavior)
		{
		case INPUTBEHAVIOR::MOVE_LEFT:
			PuzzleActor::MoveAllYouBehavior(int2::Left);
			PuzzleActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_RIGHT:
			PuzzleActor::MoveAllYouBehavior(int2::Right);
			PuzzleActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_UP:
			PuzzleActor::MoveAllYouBehavior(int2::Up);
			PuzzleActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_DOWN:
			PuzzleActor::MoveAllYouBehavior(int2::Down);
			PuzzleActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::UNDO:
			PuzzleActor::AllActorUndo();

			for (size_t i = 0; i < vecDefineActors.size(); i++)
			{
				if (true == vecDefineActors[i].IsRemove)
				{
					vecDefineActors[i].ActorData->RemoveDefine(vecDefineActors[i].RemoveDefine);
				}
				else
				{
					vecDefineActors[i].ActorData->AddDefine(vecDefineActors[i].RemoveDefine);
				}
			}
			vecDefineActors.clear();

			return;
		case INPUTBEHAVIOR::WAIT:
			PuzzleActor::MoveAllMoveBehavior();
			break;
		default:
			MsgAssert("잘못된 Input enum값 입니다.");
			break;
		}

		if (false == PuzzleActor::IsAnyMove())
		{
			return;
		}

		PuzzleActor::PuzzleActorDeathCheck();

		for (size_t i = 0; i < vecDefineActors.size(); i++)
		{
			if (true == vecDefineActors[i].IsRemove)
			{
				vecDefineActors[i].ActorData->RemoveDefine(vecDefineActors[i].RemoveDefine);
			}
			else
			{
				vecDefineActors[i].ActorData->AddDefine(vecDefineActors[i].RemoveDefine);
			}
		}

		PuzzleActor::PuzzleActorSaveBehavior();

		vecDefineActors.clear();
	}
}

void PuzzleActorManager::LoadData(const std::string_view& _PuzzleName)
{
	Reset();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("Data");
	Dir.Move("Data");
	Dir.Move("Map");

	std::vector<std::vector<int>> LoadData;
	std::vector<std::vector<int>> LoadDir;
	
	if (false == ContentDataLoader::LoadMapData(Dir.GetPlusFileName(_PuzzleName).GetPathToString(), LoadData, LoadDir))
	{
		MsgAssert("맵 데이터를 불러오는데 실패했습니다.");
		return;
	}

	int2 MapSize = { static_cast<int>(LoadData[0].size()), static_cast<int>(LoadData.size())};

	// Todo : DataLoad System
	PuzzleActor::SetGridLength(MapSize);

	float4 TotalGridSize = { ContentConst::ACTOR_SIZE.x * MapSize.x, ContentConst::ACTOR_SIZE.y * MapSize.y };
	float4 WindowSize = GameEngineWindow::GetScreenSize();
	float4 DiffSize = WindowSize - TotalGridSize;

	GridBackActor->SetScale(TotalGridSize);

	for (size_t y = 0; y < MapSize.y; y++)
	{
		for (size_t x = 0; x < MapSize.x; x++)
		{
			if (0 <= LoadData[y][x])
			{
				PuzzleActor* ActorData = PuzzleActor::CreatePuzzlActor(LoadData[y][x]);

				if (nullptr == ActorData)
				{
					MsgAssert("nullptr Actor가 반환되었습니다.");
				}

				ActorData->ResetValues();
				ActorData->LoadData(LoadData[y][x], static_cast<DIR_FLAG>(LoadDir[y][x]), true);
				ActorData->On();
				ActorData->SetGrid({static_cast<int>(x), static_cast<int>(y)});
			}
		}
	}

	PuzzleActor::AllActorRuleCheck();
	MainPuzzleLevel->SetCameraPos(-DiffSize.half());
}

void PuzzleActorManager::AddDefine(PuzzleActor* _Actor, ACTOR_DEFINE _Define, bool _IsRemove)
{
	vecDefineActors.push_back({ _Actor, _Define, _IsRemove });
}

void PuzzleActorManager::clear()
{
	PuzzleActor::AnyMoveCheckReset();
}

void PuzzleActorManager::Reset()
{
	PuzzleActor::ResetPuzzleActor();
	listInputBuffer.clear();
	vecDefineActors.clear();
}


bool PuzzleActorManager::IsPuzzleEnd() const
{
	return PuzzleActor::IsWin();
}