#include "GridActorManager.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "GridActor.h"
#include "CongratulationsUI.h"

// Todo : Data Save/Load System이 생성되면 제거
int arrDatas[18][32] = 
{
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,-1,-1, 14, 28, 31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6, 6,-1, 5, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6, 6, 6,-1, 5, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
};

bool GridActorManager::UpdateBehavior = false;

GridActorManager::GridActorManager(GameEngineLevel* _PuzzleLevel)
{
	PuzzleLevel = _PuzzleLevel;
	Init(PuzzleLevel);
}

GridActorManager::~GridActorManager()
{
	GridActor::DeleteGridActor();
}

void GridActorManager::Input(float _DT)
{
	if (true == GridActor::WinCheckValue)
	{
		return;
	}

	int2 MoveDir = int2::Zero;

	if (GameEngineInput::IsDown("ArrowUp"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::MOVE_UP);
	}
	else if (GameEngineInput::IsDown("ArrowDown"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::MOVE_DOWN);
	}
	else if (GameEngineInput::IsDown("ArrowLeft"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::MOVE_LEFT);
	}
	else if (GameEngineInput::IsDown("ArrowRight"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::MOVE_RIGHT);
	}
	else if (GameEngineInput::IsDown("Undo"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::UNDO);
	}
	else if (GameEngineInput::IsDown("Wait"))
	{
		vecWaitInputs.push_back(INPUTBEHAVIOR::WAIT);
	}
	
	if (false == GridActor::AnyActorMoveCheck && 0 < vecWaitInputs.size())
	{
		INPUTBEHAVIOR NextBehavior = vecWaitInputs.front();
		vecWaitInputs.pop_front();

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

			for (GridActor* Data : vecActors)
			{
				if (nullptr == Data)
				{
					MsgAssert("nullptr GridActor Data를 참조하려 했습니다.");
					return;
				}

				Data->Undo();
			}

			return;
		case INPUTBEHAVIOR::WAIT:
			GridActor::MoveAllMoveBehavior();
			break;
		default:
			MsgAssert("잘못된 Input enum값 입니다.");
			break;
		}

		for (GridActor* Data : vecActors)
		{
			if (nullptr == Data)
			{
				MsgAssert("nullptr GridActor Data를 참조하려 했습니다.");
				return;
			}

			Data->SaveBehaviorInfo();
		}
	}
}

void GridActorManager::Init(GameEngineLevel* _PuzzleLevel)
{
	GridActor::InitGridActor(_PuzzleLevel);
	vecActors.reserve(ContentConst::GRID_SIZE_X * ContentConst::GRID_SIZE_Y);
}

void GridActorManager::LoadData(const std::string_view& _PuzzleName)
{
	Reset();

	// Todo : DataLoad System

	for (size_t y = 0; y < 18; y++)
	{
		for (size_t x = 0; x < 32; x++)
		{
			if (0 <= arrDatas[y][x])
			{
				GridActor* ActorData = GridActor::GetActor(static_cast<TEMP_ACTOR_TYPE>(arrDatas[y][x]));

				if (nullptr == ActorData)
				{
					MsgAssert("nullptr Actor가 반환되었습니다.");
				}

				ActorData->LoadData(static_cast<TEMP_ACTOR_TYPE>(arrDatas[y][x]));
				ActorData->On();
				ActorData->SetGrid({static_cast<int>(x), static_cast<int>(y)});
				vecActors.push_back(ActorData);
			}
		}
	}
}

void GridActorManager::clear()
{
	GridActor::AnyActorMoveCheck = false;
}

void GridActorManager::Reset()
{
	GridActor::ResetGridActor();

	for (size_t i = 0; i < vecActors.size(); i++)
	{
		vecActors[i]->Off();
	}
	vecActors.clear();
}


bool GridActorManager::IsPuzzleEnd() const
{
	return GridActor::WinCheckValue;
}