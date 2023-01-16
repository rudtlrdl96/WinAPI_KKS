#include "ActorManager.h"
#include <GameEngineBase/GameEngineDebug.h>
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

ActorManager::ActorManager(GameEngineLevel* _PuzzleLevel)
{
	Init(_PuzzleLevel);
}

ActorManager::~ActorManager()
{
	GridActor::DeleteGridActor();
}

void ActorManager::Init(GameEngineLevel* _PuzzleLevel)
{
	GridActor::InitGridActor(_PuzzleLevel, ContentConst::GRID_SIZE, ContentConst::ACTOR_SIZE);
	vecActors.reserve(ContentConst::GRID_SIZE_X * ContentConst::GRID_SIZE_Y);
}

void ActorManager::LoadData(const std::string_view& _PuzzleName)
{
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