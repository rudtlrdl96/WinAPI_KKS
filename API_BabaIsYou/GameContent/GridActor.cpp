#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
std::vector<GridActor*> GridActor::vecObjectPool;
size_t GridActor::ReturnActorIndex = 0;
int2 GridActor::GridSize = int2::Zero;
float4 GridActor::ActorSize = float4::Zero;
std::vector<std::vector<GridActor::GridData>> GridActor::vecGridDatas;

#pragma region StaticFunc

GridActor* GridActor::GetActor(TEMP_ACTOR_TYPE _Type)
{
	if (ReturnActorIndex >= vecObjectPool.size())
	{
		MsgAssert("Object Pool 사이즈를 초과했습니다.");
		return nullptr;
	}

	if (nullptr == vecObjectPool[ReturnActorIndex])
	{
		MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
		return nullptr;
	}

	vecObjectPool[ReturnActorIndex]->On();
	return vecObjectPool[ReturnActorIndex++];
}

void GridActor::InitGridActor(GameEngineLevel* _PuzzleLevel, const int2& _GridSize, const float4& _ActorSize)
{
	if (nullptr != PuzzleLevel)
	{
		MsgAssert("InitGridActor를 중복 호출 하였습니다");
		return;
	}

	if (nullptr == _PuzzleLevel)
	{
		MsgAssert("nullptr PuzzleLevel이 인자로 입력되었습니다.");
		return;
	}

	vecGridDatas.resize(_GridSize.y);

	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		vecGridDatas[y].resize(_GridSize.x);

		for (size_t x = 0; x < vecGridDatas[y].size(); x++)
		{
			vecGridDatas[y][x].vecDatas.reserve(16);
		}
	}


	PuzzleLevel = _PuzzleLevel;
	GridSize = _GridSize;
	ActorSize = _ActorSize;
	vecObjectPool.reserve(GridSize.x * GridSize.y);

	for (size_t i = 0; i < vecObjectPool.capacity(); i++)
	{
		PuzzleLevel->CreateActor<GridActor>();
	}
}

void GridActor::ClearGrid()
{
	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		for (size_t x = 0; x < vecGridDatas[y].size(); x++)
		{
			vecGridDatas[y][x].clear();
		}
	}
}

void GridActor::ResetGridActor()
{
	for (size_t i = 0; i < vecObjectPool.size(); i++)
	{
		if (nullptr == vecObjectPool[i])
		{
			MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
			return;
		}

		vecObjectPool[i]->Off();
	}

	ReturnActorIndex = 0;
}

void GridActor::DeleteGridActor()
{
	vecObjectPool.clear();
	ReturnActorIndex = 0;
}

float4 GridActor::GetScreenPos(const int2& _GridPos)
{
	return {
		(ContentConst::ACTOR_SIZE.x * _GridPos.x) + ActorSize.half().x,
		(ContentConst::ACTOR_SIZE.y * _GridPos.y) + ActorSize.half().y};
}


bool GridActor::IsOver(const int2& _GridPos)
{
	if (_GridPos.x < 0 ||
		_GridPos.y < 0 ||
		_GridPos.x >= GridSize.x ||
		_GridPos.y >= GridSize.y)
	{
		return true;
	}

	return false;
}

#pragma endregion

GridActor::GridActor()
{
	vecBehaviors.reserve(32);
}

GridActor::~GridActor()
{
}

void GridActor::Start()
{
	InitRender("actor.BMP", float4::Zero, ContentConst::ACTOR_SIZE, 0, 4, 10, 24);
	vecObjectPool.push_back(this);
	Off();
}

void GridActor::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (true == IsOver(GridPos))
	{
		MsgAssert("액터가 그리드 밖으로 벗어났습니다.");
		return;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
}


void GridActor::LateUpdate(float _DT)
{

	if (true == IsDefine(DEFINE_INFO::YOU))
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

		if (0 < vecWaitInputs.size() && false == IsMove)
		{
			MoveDir = vecWaitInputs.front();
			vecWaitInputs.pop_front();

			if (true == Move(GridPos + MoveDir))
			{
				if (int2::Left == MoveDir)
				{
					vecBehaviors.push_back(BEHAVIOR::MOVE_LEFT);
				}
				else if (int2::Right == MoveDir)
				{
					vecBehaviors.push_back(BEHAVIOR::MOVE_RIGHT);
				}
				else if (int2::Up == MoveDir)
				{
					vecBehaviors.push_back(BEHAVIOR::MOVE_UP);
				}
				else if (int2::Down == MoveDir)
				{
					vecBehaviors.push_back(BEHAVIOR::MOVE_DOWN);
				}
				else
				{
					MsgAssert("잘못된 방향으로 액터가 움직였습니다.");
					return;
				}

				NextAnim();
				SetAnimDir(MoveDir);
			}
		}
	}

	if (true == IsMove)
	{
		MoveProgress += _DT * ContentConst::MOVE_SPEED;

		if (1.0f <= MoveProgress)
		{
			IsMove = false;
			MoveProgress = 1.0f;
		}

		SetPos(Lerp(GetScreenPos(PrevPos), GetScreenPos(GridPos), MoveProgress));
	}
}

void GridActor::LoadData(TEMP_ACTOR_TYPE _Actor)
{
	if (_Actor < TEMP_ACTOR_TYPE::BABA || _Actor >= TEMP_ACTOR_TYPE::COUNT)
	{
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	vecBehaviors.clear();
	vecWaitInputs.clear();

	// Todo : File Save/Load 시스템이 완성된 후 데이터베이스 로드

	// 속성 값 초기화
	DefineData = 0;

	if (TEMP_ACTOR_TYPE::BABA == _Actor)
	{
		SetFrame(1);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;
		
		// Todo : 테스트용 임시 호출 추후 데이터시스템이 생성되면 삭제
		AddDefine(DEFINE_INFO::YOU);
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::BABA_TEXT == _Actor)
	{
		SetFrame(0);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::IS_TEXT == _Actor)
	{
		SetFrame(792);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::VERB_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::STOP);
	}

	if (TEMP_ACTOR_TYPE::YOU_TEXT == _Actor)
	{
		SetFrame(864);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::PUSH);
	}

	SetAnimDir(int2::Right);
}

void GridActor::SetGrid(const int2& _Pos)
{
	GridPos = _Pos;
	SetPos(GetScreenPos(GridPos));
}

void GridActor::AddDefine(DEFINE_INFO _Info)
{
	DefineData |= static_cast<size_t>(_Info);
}

void GridActor::RemoveDefine(DEFINE_INFO _Info)
{
	DefineData &= ~static_cast<size_t>(_Info);
}

bool GridActor::IsDefine(DEFINE_INFO _Info)
{
	return DefineData & static_cast<size_t>(_Info);
}


bool GridActor::Move(const int2& _NextPos)
{
	if (true == IsMove)
	{
		return false;
	}

	if (false == CanMove(_NextPos))
	{
		return false;
	}

	PushDir(_NextPos - GridPos);
	
	IsMove = true;
	PrevPos = GridPos;
	GridPos = _NextPos;
	MoveProgress = 0.0f;

	return true;
}

void GridActor::UndoMove(const int2& _NextPos)
{

}

void GridActor::Push(const int2& _Dir)
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos + _Dir;
	MoveProgress = 0.0f;
}

void GridActor::PushDir(const int2& _Dir)
{
	int2 PushPos = GridPos + _Dir;

	while (true)
	{
		if (true == IsOver(PushPos))
		{
			break;
		}

		size_t DefineData = vecGridDatas[PushPos.y][PushPos.x].GetDefine();

		if (static_cast<size_t>(DEFINE_INFO::YOU) & DefineData)
		{
			return;
		}

		if (static_cast<size_t>(DEFINE_INFO::PUSH) & DefineData)
		{
			vecGridDatas[PushPos.y][PushPos.x].Push(_Dir);
			PushPos += _Dir;
			continue;
		}

		break;
	}
}

bool GridActor::CanMove(const int2& _NextPos)
{
	if (true == IsOver(_NextPos))
	{
		return false;
	}

	int2 Dir = _NextPos - GridPos;
	int2 CheckPos = _NextPos;

	while (true)
	{
		if (true == IsOver(CheckPos))
		{
			return false;
		}

		size_t DefineData = vecGridDatas[CheckPos.y][CheckPos.x].GetDefine();

		if (static_cast<size_t>(DEFINE_INFO::STOP) & DefineData)
		{
			return false;
		}

		if (static_cast<size_t>(DEFINE_INFO::YOU) & DefineData)
		{
			return true;
		}

		if (static_cast<size_t>(DEFINE_INFO::PUSH) & DefineData)
		{
			CheckPos += Dir;
			continue;
		}

		break;
	}

	return true;
}


void GridActor::Undo()
{
	BEHAVIOR UndoBehavior = vecBehaviors.back();

	switch (UndoBehavior)
	{
	case GridActor::BEHAVIOR::MOVE_LEFT:
		break;
	case GridActor::BEHAVIOR::MOVE_RIGHT:
		break;
	case GridActor::BEHAVIOR::MOVE_UP:
		break;
	case GridActor::BEHAVIOR::MOVE_DOWN:
		break;
	case GridActor::BEHAVIOR::SINK:
		break;
	case GridActor::BEHAVIOR::DEFEAT:
		break;
	case GridActor::BEHAVIOR::MELT:
		break;
	case GridActor::BEHAVIOR::WIN:
		break;
	default:
		MsgAssert("잘못된 Behavior Type 입니다.");
		break;
	}

	vecBehaviors.pop_back();
}