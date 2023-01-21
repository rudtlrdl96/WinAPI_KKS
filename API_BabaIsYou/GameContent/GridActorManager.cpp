#include "GridActorManager.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentConst.h"
#include "GridActor.h"
#include "CongratulationsUI.h"
#include "BlackBackUI.h"

const int TEMP_X = 20;
const int TEMP_Y = 15;

// Todo : Data Save/Load System이 생성되면 제거
int arrDatas[TEMP_Y][TEMP_X] =
{
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1, 0, 0,-1,-1, 14, 28, 31,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1, 6, 6,-1, 5, 5,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1, 6, 6, 6,-1, 5, 5,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
};

GridActorManager::GridActorManager()
{

}

GridActorManager::~GridActorManager()
{
	GridActor::DeleteGridActor();
}

void GridActorManager::Init(GameEngineLevel* _PuzzleLevel)
{
	PuzzleLevel = _PuzzleLevel;
	GridActor::InitGridActor(_PuzzleLevel);
	GridBackActor = PuzzleLevel->CreateActor<BlackBackUI>();
}

void GridActorManager::Input(float _DT)
{
	if (true == GridActor::IsWin())
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
	
	if (false == GridActor::IsAnyMove() && 0 < vecWaitInputs.size())
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
			GridActor::AllActorUndo();
			return;
		case INPUTBEHAVIOR::WAIT:
			GridActor::MoveAllMoveBehavior();
			break;
		default:
			MsgAssert("잘못된 Input enum값 입니다.");
			break;
		}

		// Todo : RuleCheck 기능 추가
		GridActor::GridActorEndCheck();
	}
}

void GridActorManager::LoadData(const std::string_view& _PuzzleName)
{
	Reset();

	// Todo : DataLoad System
	GridActor::SetGridLength(int2{ TEMP_X, TEMP_Y});

	float4 TotalGridSize = { ContentConst::ACTOR_SIZE.x * TEMP_X, ContentConst::ACTOR_SIZE.y * TEMP_Y };
	float4 WindowSize = GameEngineWindow::GetScreenSize();
	float4 DiffSize = WindowSize - TotalGridSize;

	GridBackActor->SetScale(TotalGridSize);

	for (size_t y = 0; y < TEMP_Y; y++)
	{
		for (size_t x = 0; x < TEMP_X; x++)
		{
			if (0 <= arrDatas[y][x])
			{
				GridActor* ActorData = GridActor::CreateGridActor(static_cast<TEMP_ACTOR_INDEX>(arrDatas[y][x]));

				if (nullptr == ActorData)
				{
					MsgAssert("nullptr Actor가 반환되었습니다.");
				}

				ActorData->ResetValues();
				ActorData->LoadData(static_cast<TEMP_ACTOR_INDEX>(arrDatas[y][x]));
				ActorData->On();
				ActorData->SetGrid({static_cast<int>(x), static_cast<int>(y)});
			}
		}
	}

	PuzzleLevel->SetCameraPos(-DiffSize.half());
}

void GridActorManager::clear()
{
	GridActor::AnyMoveCheckReset();
}

void GridActorManager::Reset()
{
	GridActor::ResetGridActor();
}


bool GridActorManager::IsPuzzleEnd() const
{
	return GridActor::IsWin();
}