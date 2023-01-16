#include "ActorManager.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "GridActor.h";

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


void ActorManager::Update(float _Time)
{
	clear();
}

void ActorManager::LateUpdate(float _Time)
{
	int2 MoveDir = int2::Zero;

	if (GameEngineInput::IsDown("ArrowUp"))
	{
		MoveDir = int2::Up;
	}
	if (GameEngineInput::IsDown("ArrowDown"))
	{
		MoveDir = int2::Down;
	}
	if (GameEngineInput::IsDown("ArrowLeft"))
	{
		MoveDir = int2::Left;
	}
	if (GameEngineInput::IsDown("ArrowRight"))
	{
		MoveDir = int2::Right;
	}

	if (int2::Zero != MoveDir && 3 > vecWaitInputs.size())
	{
		vecWaitInputs.push_back(MoveDir);
	}

	if (0 < vecWaitInputs.size())
	{
		MoveDir = vecWaitInputs.front();
		vecWaitInputs.pop_front();

		for (GridActor* ActorData : vecActors)
		{
			if (nullptr == ActorData)
			{
				MsgAssert("nullptr GridActor를 참조하려 했습니다");
				return;
			}

			if (ActorData->IsDefine(GridActor::DEFINE_INFO::YOU))
			{
				ActorData->Move(MoveDir);
			}

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