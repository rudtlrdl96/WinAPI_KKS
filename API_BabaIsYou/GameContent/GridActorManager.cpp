#include "GridActorManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentConst.h"
#include "GridActor.h"
#include "CongratulationsUI.h"
#include "BlackBackUI.h"
#include "PuzzleLevel.h"
#include "ContentDataLoader.h"

GridActorManager::GridActorManager()
{

}

GridActorManager::~GridActorManager()
{
}

void GridActorManager::Init(PuzzleLevel* _PuzzleLevel)
{
	MainPuzzleLevel = _PuzzleLevel;
	GridActor::InitGridActor(_PuzzleLevel);
	GridBackActor = MainPuzzleLevel->CreateActor<BlackBackUI>();
}

void GridActorManager::Input(float _DT)
{
	if (true == GridActor::IsWin())
	{
		return;
	}
	GridActor::GetActors(ACTOR_DEFINE::YOU);
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
	
	if (false == GridActor::IsAnyMove() && 0 < listInputBuffer.size())
	{
		INPUTBEHAVIOR NextBehavior = listInputBuffer.front();
		listInputBuffer.pop_front();

		switch (NextBehavior)
		{
		case INPUTBEHAVIOR::MOVE_LEFT:
			GridActor::MoveAllYouBehavior(int2::Left);
			GridActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_RIGHT:
			GridActor::MoveAllYouBehavior(int2::Right);
			GridActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_UP:
			GridActor::MoveAllYouBehavior(int2::Up);
			GridActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::MOVE_DOWN:
			GridActor::MoveAllYouBehavior(int2::Down);
			GridActor::MoveAllMoveBehavior();
			break;
		case INPUTBEHAVIOR::UNDO:
			GridActor::AllActorUndo();

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
			GridActor::MoveAllMoveBehavior();
			break;
		default:
			MsgAssert("잘못된 Input enum값 입니다.");
			break;
		}

		if (false == GridActor::IsAnyMove())
		{
			return;
		}

		GridActor::GridActorDeathCheck();

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

		GridActor::GridActorSaveBehavior();

		vecDefineActors.clear();
	}
}

void GridActorManager::LoadData(const std::string_view& _PuzzleName)
{
	Reset();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("Data");
	Dir.Move("Data");
	Dir.Move("Map");

	std::vector<std::vector<int>> LoadData;
	
	if (false == ContentDataLoader::LoadMapData(Dir.GetPlusFileName(_PuzzleName).GetPathToString(), LoadData))
	{
		MsgAssert("맵 데이터를 불러오는데 실패했습니다.");
		return;
	}

	int2 MapSize = { static_cast<int>(LoadData[0].size()), static_cast<int>(LoadData.size())};

	// Todo : DataLoad System
	GridActor::SetGridLength(MapSize);

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
				GridActor* ActorData = GridActor::CreateGridActor(LoadData[y][x]);

				if (nullptr == ActorData)
				{
					MsgAssert("nullptr Actor가 반환되었습니다.");
				}

				ActorData->ResetValues();
				ActorData->LoadData(LoadData[y][x], true);
				ActorData->On();
				ActorData->SetGrid({static_cast<int>(x), static_cast<int>(y)});
			}
		}
	}

	GridActor::AllActorRuleCheck();
	MainPuzzleLevel->SetCameraPos(-DiffSize.half());
}

void GridActorManager::AddDefine(GridActor* _Actor, ACTOR_DEFINE _Define, bool _IsRemove)
{
	vecDefineActors.push_back({ _Actor, _Define, _IsRemove });
}

void GridActorManager::clear()
{
	GridActor::AnyMoveCheckReset();
}

void GridActorManager::Reset()
{
	GridActor::ResetGridActor();
	listInputBuffer.clear();
	vecDefineActors.clear();
}


bool GridActorManager::IsPuzzleEnd() const
{
	return GridActor::IsWin();
}