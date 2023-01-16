#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"

/// GridData

#pragma region GridData

void GridActor::GridData::push_back(GridActor* _Actor)
{
	vecDatas.push_back(_Actor);
}

void GridActor::GridData::clear()
{
	PushDoubleCheck = false;
	vecDatas.clear();
}

void GridActor::GridData::Push(const int2& _Dir)
{
	if (true == PushDoubleCheck)
	{
		return;
	}

	for (GridActor* Data : vecDatas)
	{
		if (true == Data->IsDefine(DEFINE_INFO::PUSH))
		{
			Data->SetDir(_Dir);
			Data->Push();
			PushDoubleCheck = true;
		}
	}
}

size_t GridActor::GridData::GetDefine()
{
	size_t Info = static_cast<size_t>(DEFINE_INFO::NONE);

	for (GridActor* Data : vecDatas)
	{
		Info |= Data->GetDefine();
	}

	return Info;
}


#pragma endregion

/// static GridActor
#pragma region StaticFunc

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
size_t GridActor::ReturnActorIndex = 0;
int2 GridActor::GridSize = int2::Zero;
float4 GridActor::ActorSize = float4::Zero;
bool GridActor::AnyActorMoveCheck = false;

std::vector<GridActor*> GridActor::vecObjectPool;
std::vector<std::vector<GridActor::GridData>> GridActor::vecGridDatas;
std::vector<GridActor*> GridActor::vecYouBehaviors;
std::vector<GridActor*> GridActor::vecMoveBehaviors;

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

	vecYouBehaviors.reserve(GridSize.x * GridSize.y);
	vecMoveBehaviors.reserve(GridSize.x * GridSize.y);
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

	vecYouBehaviors.clear();
	vecMoveBehaviors.clear();
	AnyActorMoveCheck = false;
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


void GridActor::MoveAllYouBehavior(const int2& _Dir)
{
	for (GridActor* Data: vecYouBehaviors)
	{
		if (nullptr == Data)
		{
			MsgAssert("nullptr GridActor Data를 참조하려 합니다.");
			return;
		}

		Data->SetDir(_Dir);
		Data->Move();
	}
}

void GridActor::MoveAllMoveBehavior()
{
	for (GridActor* Data : vecMoveBehaviors)
	{
		if (nullptr == Data)
		{
			MsgAssert("nullptr GridActor Data를 참조하려 합니다.");
			return;
		}

		Data->Move();
	}
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

/// static GridActor

GridActor::GridActor()
{
	vecBehaviors.reserve(32);
	CurFramesBehaviors.reserve(4);
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
	if (true == IsDefine(DEFINE_INFO::YOU))
	{
		vecYouBehaviors.push_back(this);
	}
	else if (true == IsDefine(DEFINE_INFO::MOVE))
	{
		vecMoveBehaviors.push_back(this);
	}

	WiggleActor::Update(_DT);

	if (true == IsOver(GridPos))
	{
		MsgAssert("액터가 그리드 밖으로 벗어났습니다.");
		return;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);

	if (true == IsMove)
	{
		AnyActorMoveCheck = true;
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

void GridActor::SaveBehaviorInfo()
{
	if (0 >= CurFramesBehaviors.size())
	{
		CurFramesBehaviors.push_back(BEHAVIOR::WAIT);
	}

	vecBehaviors.push_back(CurFramesBehaviors);
	CurFramesBehaviors.clear();
}

void GridActor::Undo()
{
	if (0 >= vecBehaviors.size())
	{
		return;
	}

	const std::vector<BEHAVIOR>& vecUndos = vecBehaviors.back();

	for (int i = static_cast<int>(vecUndos.size() - 1); i >= 0 ; --i)
	{
		switch (vecUndos[i])
		{
		case BEHAVIOR::WAIT:
			break;
		case BEHAVIOR::MOVE:
			UnMove();
			break;
		case BEHAVIOR::PUSH:
			UnPush();
			break;
		case BEHAVIOR::TURN_LEFT:
			UnTurnLeft();
			break;
		case BEHAVIOR::TURN_RIGHT:
			UnTurnRight();
			break;
		case BEHAVIOR::SINK:
			break;
		case BEHAVIOR::DEFEAT:
			break;
		case BEHAVIOR::MELT:
			break;
		case BEHAVIOR::WIN:
			break;
		default:
			MsgAssert("잘못된 Behavior Type 입니다.");
			break;
		}
	}

	vecBehaviors.pop_back();
}

bool GridActor::Move()
{
	if (true == IsMove)
	{
		return false;
	}

	if (false == CanMove(GridPos + MoveDir))
	{
		return false;
	}

	AllPushDir(MoveDir);
	
	IsMove = true;
	PrevPos = GridPos;
	GridPos += MoveDir;
	MoveProgress = 0.0f;

	CurFramesBehaviors.push_back(BEHAVIOR::MOVE);

	NextAnim();

	return true;
}

void GridActor::UnMove()
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos -= MoveDir;
	MoveProgress = 0.0f;

	PrevAnim();
}


void GridActor::SetDir(const int2& _Dir)
{
	while (_Dir != MoveDir)
	{
		TurnLeft();
	} 
}

void GridActor::TurnLeft()
{
	CurFramesBehaviors.push_back(BEHAVIOR::TURN_LEFT);

	if (int2::Left == MoveDir)
	{
		MoveDir = int2::Down;
	}
	else if (int2::Down == MoveDir)
	{
		MoveDir = int2::Right;
	}
	else if (int2::Right == MoveDir)
	{
		MoveDir = int2::Up;
	}
	else if (int2::Up == MoveDir)
	{
		MoveDir = int2::Left;
	}

	SetAnimDir(MoveDir);
}

void GridActor::UnTurnLeft()
{
	if (int2::Left == MoveDir)
	{
		MoveDir = int2::Up;
	}
	else if (int2::Down == MoveDir)
	{
		MoveDir = int2::Left;
	}
	else if (int2::Right == MoveDir)
	{
		MoveDir = int2::Down;
	}
	else if (int2::Up == MoveDir)
	{
		MoveDir = int2::Right;
	}

	SetAnimDir(MoveDir);
}

void GridActor::TurnRight()
{
	CurFramesBehaviors.push_back(BEHAVIOR::TURN_RIGHT);

	if (int2::Left == MoveDir)
	{
		MoveDir = int2::Up;
	}
	else if (int2::Down == MoveDir)
	{
		MoveDir = int2::Left;
	}
	else if (int2::Right == MoveDir)
	{
		MoveDir = int2::Down;
	}
	else if (int2::Up == MoveDir)
	{
		MoveDir = int2::Right;
	}

	SetAnimDir(MoveDir);
}

void GridActor::UnTurnRight()
{
	if (int2::Left == MoveDir)
	{
		MoveDir = int2::Down;
	}
	else if (int2::Down == MoveDir)
	{
		MoveDir = int2::Right;
	}
	else if (int2::Right == MoveDir)
	{
		MoveDir = int2::Up;
	}
	else if (int2::Up == MoveDir)
	{
		MoveDir = int2::Left;
	}

	SetAnimDir(MoveDir);
}

void GridActor::Push()
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos + MoveDir;
	MoveProgress = 0.0f;

	CurFramesBehaviors.push_back(BEHAVIOR::PUSH);
}

void GridActor::UnPush()
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos - MoveDir;
	MoveProgress = 0.0f;
}

void GridActor::AllPushDir(const int2& _Dir)
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
