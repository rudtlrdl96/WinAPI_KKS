#include "ActorManager.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "GridActor.h"

// Todo : Data Save/Load System이 생성되면 제거
int arrDatas[18][32] = 
{
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,-1,-1, 14, 28, 31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
};

bool ActorManager::UpdateBehavior = false;

ActorManager::ActorManager(GameEngineLevel* _PuzzleLevel)
{
	Init(_PuzzleLevel);
}

ActorManager::~ActorManager()
{
	GridActor::DeleteGridActor();
}

void ActorManager::Input(float _DT)
{
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
		case ActorManager::INPUTBEHAVIOR::MOVE_LEFT:
			GridActor::MoveAllYouBehavior(int2::Left);
			GridActor::MoveAllMoveBehavior();
			break;
		case ActorManager::INPUTBEHAVIOR::MOVE_RIGHT:
			GridActor::MoveAllYouBehavior(int2::Right);
			GridActor::MoveAllMoveBehavior();
			break;
		case ActorManager::INPUTBEHAVIOR::MOVE_UP:
			GridActor::MoveAllYouBehavior(int2::Up);
			GridActor::MoveAllMoveBehavior();
			break;
		case ActorManager::INPUTBEHAVIOR::MOVE_DOWN:
			GridActor::MoveAllYouBehavior(int2::Down);
			GridActor::MoveAllMoveBehavior();
			break;
		case ActorManager::INPUTBEHAVIOR::UNDO:

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
		case ActorManager::INPUTBEHAVIOR::WAIT:
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

void ActorManager::Init(GameEngineLevel* _PuzzleLevel)
{
	GridActor::InitGridActor(_PuzzleLevel, ContentConst::GRID_SIZE, ContentConst::ACTOR_SIZE);
	vecActors.reserve(ContentConst::GRID_SIZE_X * ContentConst::GRID_SIZE_Y);
}

void ActorManager::LoadData(const std::string_view& _PuzzleName)
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

void ActorManager::clear()
{
	GridActor::ClearGrid();
}

void ActorManager::Reset()
{
	GridActor::ResetGridActor();

	for (size_t i = 0; i < vecActors.size(); i++)
	{
		vecActors[i]->Off();
	}
	vecActors.clear();
}