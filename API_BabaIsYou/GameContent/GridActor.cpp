#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"

/// GridData

#pragma region GridData

void GridActor::GridData::push_back(GridActor* _Actor)
{
	mapDatas[_Actor->ActorKey] = _Actor;
}

void GridActor::GridData::erase(GridActor* _Actor)
{
	std::map<int, GridActor*>::iterator FindIter = mapDatas.find(_Actor->ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}
}

void GridActor::GridData::clear()
{
	mapDatas.clear();
}

void GridActor::GridData::Push(const int2& _Pos, const int2& _Dir, bool _IsInputMove)
{
	if (0 >= mapDatas.size())
	{
		return;
	}

	std::vector<GridActor*> vecPushErase;
	vecPushErase.reserve(mapDatas.size());

	int2 NextPos = _Pos + _Dir;

	for (const std::pair<int, GridActor*>& Data : mapDatas)
	{
		if (true == Data.second->IsDeath)
		{
			continue;
		}

		if ((false == _IsInputMove || false == Data.second->IsDefine(DEFINE_INFO::YOU)) &&
			true == Data.second->IsDefine(DEFINE_INFO::PUSH))
		{
			vecPushErase.push_back(Data.second);
		}
	}

	for (GridActor* Data : vecPushErase)
	{
		Data->SetDir(_Dir);
		Data->Push();
	}
	
	vecGridDatas[NextPos.y][NextPos.x].DeathCheck();
}


void GridActor::GridData::DeathCheck()
{
	size_t GridDefine = GetDefine();

	std::map<int, GridActor*>::iterator LoopIter = mapDatas.begin();
	std::map<int, GridActor*>::iterator EndIter = mapDatas.end();

	bool SinkCheck = 1 < mapDatas.size();

	while (LoopIter != EndIter)
	{ 
		if (LoopIter->second->IsDefine(DEFINE_INFO::YOU) && static_cast<size_t>(DEFINE_INFO::DEFEAT) & GridDefine)
		{
			LoopIter->second->ActorDeath();
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if (LoopIter->second->IsDefine(DEFINE_INFO::HOT) && static_cast<size_t>(DEFINE_INFO::MELT) & GridDefine)
		{
			LoopIter->second->ActorDeath();
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if (true == SinkCheck && static_cast<size_t>(DEFINE_INFO::SINK) & GridDefine)
		{
			LoopIter->second->ActorDeath();
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}

		++LoopIter;
	}
}

size_t GridActor::GridData::GetDefine()
{
	size_t Info = static_cast<size_t>(DEFINE_INFO::NONE);

	for (const std::pair<int, GridActor*>& Data : mapDatas)
	{
		if (true == Data.second->IsDeath)
		{
			continue;
		}

		Info |= Data.second->GetDefine();
	}

	return Info;
}

#pragma endregion

/// static GridActor
#pragma region StaticFunc

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
size_t GridActor::ObjectPoolCount = 0;
bool GridActor::AnyActorMoveCheck = false;
int GridActor::NextActorKey = 0;

std::vector<GridActor*> GridActor::vecObjectPool;
std::vector<std::vector<GridActor::GridData>> GridActor::vecGridDatas;
std::map<GridActor::DEFINE_INFO, std::map<int, GridActor*>> GridActor::mapDefineActorDatas;

GridActor* GridActor::GetActor(TEMP_ACTOR_TYPE _Type)
{
	if (ObjectPoolCount >= vecObjectPool.size())
	{
		MsgAssert("Object Pool ����� �ʰ��߽��ϴ�.");
		return nullptr;
	}

	if (nullptr == vecObjectPool[ObjectPoolCount])
	{
		MsgAssert("vecObjectPool ���Ϳ� nullptr Actor�� �����մϴ�.");
		return nullptr;
	}

	vecObjectPool[ObjectPoolCount]->On();
	return vecObjectPool[ObjectPoolCount++];
}

void GridActor::InitGridActor(GameEngineLevel* _PuzzleLevel)
{
	if (nullptr != PuzzleLevel)
	{
		MsgAssert("InitGridActor�� �ߺ� ȣ�� �Ͽ����ϴ�");
		return;
	}

	if (nullptr == _PuzzleLevel)
	{
		MsgAssert("nullptr PuzzleLevel�� ���ڷ� �ԷµǾ����ϴ�.");
		return;
	}

	vecGridDatas.resize(ContentConst::GRID_SIZE.y);

	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		vecGridDatas[y].resize(ContentConst::GRID_SIZE.x);
	}

	PuzzleLevel = _PuzzleLevel;

	vecObjectPool.reserve(ContentConst::GRID_SIZE.x * ContentConst::GRID_SIZE.y);

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

	AnyActorMoveCheck = false;
}

void GridActor::ResetGridActor()
{
	for (size_t i = 0; i < vecObjectPool.size(); i++)
	{
		if (nullptr == vecObjectPool[i])
		{
			MsgAssert("vecObjectPool ���Ϳ� nullptr Actor�� �����մϴ�.");
			return;
		}

		vecObjectPool[i]->Off();
	}

	ObjectPoolCount = 0;
}

void GridActor::DeleteGridActor()
{
	vecObjectPool.clear();
	ObjectPoolCount = 0;
}

void GridActor::MoveAllYouBehavior(const int2& _Dir)
{
	for (const std::pair<int, GridActor*>& Data: mapDefineActorDatas[DEFINE_INFO::YOU])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr GridActor Data�� �����Ϸ� �մϴ�.");
			return;
		}

		Data.second->SetDir(_Dir);
		Data.second->Move(true);
	}
}

void GridActor::MoveAllMoveBehavior()
{
	for (const std::pair<int, GridActor*>& Data : mapDefineActorDatas[DEFINE_INFO::MOVE])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr GridActor Data�� �����Ϸ� �մϴ�.");
			return;
		}

		Data.second->Move(false);
	}
}

float4 GridActor::GetScreenPos(const int2& _GridPos)
{
	return {
		(ContentConst::ACTOR_SIZE.x * _GridPos.x) + ContentConst::ACTOR_SIZE.half().x,
		(ContentConst::ACTOR_SIZE.y * _GridPos.y) + ContentConst::ACTOR_SIZE.half().y};
}


bool GridActor::IsOver(const int2& _GridPos)
{
	if (_GridPos.x < 0 ||
		_GridPos.y < 0 ||
		_GridPos.x >= ContentConst::GRID_SIZE.x ||
		_GridPos.y >= ContentConst::GRID_SIZE.y)
	{
		return true;
	}

	return false;
}

#pragma endregion

/// static GridActor

GridActor::GridActor() :
	ActorKey(NextActorKey++)
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
	WiggleActor::Update(_DT);

	if (true == IsOver(GridPos))
	{
		MsgAssert("���Ͱ� �׸��� ������ ������ϴ�.");
		return;
	}

	if (false == IsDeath && true == IsMove)
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
		MsgAssert("�߸��� Actor Type�� �ԷµǾ����ϴ�.");
		return;
	}

	vecBehaviors.clear();

	// Todo : File Save/Load �ý����� �ϼ��� �� �����ͺ��̽� �ε�

	// �Ӽ� �� �ʱ�ȭ
	DefineData = 0;

	if (TEMP_ACTOR_TYPE::BABA == _Actor)
	{
		SetFrame(2);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;

		// Todo : �׽�Ʈ�� �ӽ� ȣ�� ���� �����ͽý����� �����Ǹ� ����
		AddDefine(DEFINE_INFO::YOU);
		AddDefine(DEFINE_INFO::PUSH);
		AddDefine(DEFINE_INFO::HOT);
	}

	if (TEMP_ACTOR_TYPE::KEKE == _Actor)
	{
		SetFrame(74);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;

		// Todo : �׽�Ʈ�� �ӽ� ȣ�� ���� �����ͽý����� �����Ǹ� ����
		//AddDefine(DEFINE_INFO::PUSH);
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

	if (TEMP_ACTOR_TYPE::LAVA == _Actor)
	{
		SetFrame(434);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::TILE;
		AddDefine(DEFINE_INFO::MELT);
	}

	if (TEMP_ACTOR_TYPE::WATER == _Actor)
	{
		SetFrame(362);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::TILE;
		AddDefine(DEFINE_INFO::SINK);
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::SKULL == _Actor)
	{
		SetFrame(722);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::DYNAMIC;
		AddDefine(DEFINE_INFO::DEFEAT);
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::WIN_TEXT == _Actor)
	{
		SetFrame(866);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::DEFEAT);
	}

	SetAnimDir(int2::Right);
}

void GridActor::SetGrid(const int2& _Pos)
{
	if (false == IsOver(PrevPos))
	{
		vecGridDatas[PrevPos.y][PrevPos.x].erase(this);
	}

	GridPos = _Pos;
	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	SetPos(GetScreenPos(GridPos));
}

void GridActor::AddDefine(DEFINE_INFO _Info)
{
	mapDefineActorDatas[_Info][ActorKey] = this;
	DefineData |= static_cast<size_t>(_Info);
}

void GridActor::RemoveDefine(DEFINE_INFO _Info)
{
	std::map<int, GridActor*>& mapDatas = mapDefineActorDatas[_Info];
	std::map<int, GridActor*>::iterator FindIter = mapDatas.find(ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}

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
			UndoMove();
			break;
		case BEHAVIOR::PUSH:
			UndoPush();
			break;
		case BEHAVIOR::TURN_LEFT:
			UndoTurnLeft();
			break;
		case BEHAVIOR::TURN_RIGHT:
			UndoTurnRight();
			break;
		case BEHAVIOR::DEATH:
			UndoActorDeath();
			break;
		case BEHAVIOR::WIN:
			break;
		default:
			MsgAssert("�߸��� Behavior Type �Դϴ�.");
			break;
		}
	}

	vecBehaviors.pop_back();
}

bool GridActor::Move(bool _IsInputMove)
{
	if (true == IsDeath)
	{
		return false;
	}

	if (false == CanMove(GridPos + MoveDir))
	{
		return false;
	}

	AllPushDir(MoveDir, _IsInputMove);
	
	IsMove = true;
	PrevPos = GridPos;
	GridPos += MoveDir;
	MoveProgress = 0.0f;

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);
	vecGridDatas[GridPos.y][GridPos.x].DeathCheck();

	CurFramesBehaviors.push_back(BEHAVIOR::MOVE);
	NextAnim();

	return true;
}

void GridActor::UndoMove()
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos -= MoveDir;
	MoveProgress = 0.0f;

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	PrevAnim();
}

void GridActor::Push()
{
	if (true == IsDeath)
	{
		return;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos + MoveDir;
	MoveProgress = 0.0f;

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	CurFramesBehaviors.push_back(BEHAVIOR::PUSH);
}

void GridActor::UndoPush()
{
	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos - MoveDir;
	MoveProgress = 0.0f;

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

}

void GridActor::TurnLeft()
{
	if (true == IsDeath)
	{
		return;
	}

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

void GridActor::UndoTurnLeft()
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
	if (true == IsDeath)
	{
		return;
	}

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

void GridActor::UndoTurnRight()
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

void GridActor::SetDir(const int2& _Dir)
{
	if (true == IsDeath)
	{
		return;
	}

	while (_Dir != MoveDir)
	{
		TurnLeft();
	} 
}

void GridActor::ActorDeath()
{
	if (true == IsDeath)
	{
		MsgAssert("�̹� ���������� GridActor�Դϴ�.");
		return;
	}

	IsDeath = true;
	CurFramesBehaviors.push_back(BEHAVIOR::DEATH);

	RenderOff();
}

void GridActor::UndoActorDeath()
{
	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	IsDeath = false;
	RenderOn();
}

void GridActor::AllPushDir(const int2& _Dir, bool _IsInputMove)
{
	int2 PushPos = GridPos + _Dir;

	while (false == IsOver(PushPos))
	{
		size_t DefineData = vecGridDatas[PushPos.y][PushPos.x].GetDefine();

		if (static_cast<size_t>(DEFINE_INFO::YOU) & DefineData)
		{
			break;
		}

		if (static_cast<size_t>(DEFINE_INFO::PUSH) & DefineData)
		{
			PushPos += _Dir;
			continue;
		}

		break;
	}

	while (GridPos != PushPos)
	{
		size_t DefineData = vecGridDatas[PushPos.y][PushPos.x].GetDefine();
		vecGridDatas[PushPos.y][PushPos.x].Push(PushPos, _Dir, _IsInputMove);
		PushPos -= _Dir;
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

	while (false == IsOver(CheckPos))
	{
		size_t DefineData = vecGridDatas[CheckPos.y][CheckPos.x].GetDefine();

		if (static_cast<size_t>(DEFINE_INFO::STOP) & DefineData)
		{
			return false;
		}

		if (static_cast<size_t>(DEFINE_INFO::PUSH) & DefineData)
		{
			CheckPos += Dir;
			continue;
		}

		if (static_cast<size_t>(DEFINE_INFO::YOU) & DefineData)
		{
			return true;
		}

		break;
	}

	return true;
}
