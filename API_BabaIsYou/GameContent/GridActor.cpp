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
	
	if (false == IsOver(NextPos))
	{
		vecGridDatas[NextPos.y][NextPos.x].DeathCheck();
	}
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

bool GridActor::GridData::equals(const std::string_view& _Name)
{
	for (const std::pair<int, GridActor*>& Data : mapDatas)
	{
		if (true == Data.second->IsDeath)
		{
			continue;
		}

		if (_Name == Data.second->ActorName)
		{
			return true;
		}
	}

	return false;
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
std::map<int, int> GridActor::mapTileRenderIndex;

GridActor* GridActor::GetActor(TEMP_ACTOR_TYPE _Type)
{
	if (ObjectPoolCount >= vecObjectPool.size())
	{
		MsgAssert("Object Pool 사이즈를 초과했습니다.");
		return nullptr;
	}

	if (nullptr == vecObjectPool[ObjectPoolCount])
	{
		MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
		return nullptr;
	}

	vecObjectPool[ObjectPoolCount]->On();
	return vecObjectPool[ObjectPoolCount++];
}

void GridActor::InitGridActor(GameEngineLevel* _PuzzleLevel)
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

	mapTileRenderIndex[DIR_FLAG::NONE] = 0;
	mapTileRenderIndex[DIR_FLAG::RIGHT] = 1;
	mapTileRenderIndex[DIR_FLAG::UP] = 2;
	mapTileRenderIndex[DIR_FLAG::LEFT] = 4;
	mapTileRenderIndex[DIR_FLAG::DOWN] = 8;
	mapTileRenderIndex[DIR_FLAG::RIGHT | DIR_FLAG::UP] = 3;
	mapTileRenderIndex[DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 5;
	mapTileRenderIndex[DIR_FLAG::LEFT | DIR_FLAG::UP] = 6;
	mapTileRenderIndex[DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 9;
	mapTileRenderIndex[DIR_FLAG::UP | DIR_FLAG::DOWN] = 10;
	mapTileRenderIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN] = 12;
	mapTileRenderIndex[DIR_FLAG::LEFT | DIR_FLAG::UP | DIR_FLAG::RIGHT] = 7;
	mapTileRenderIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 11;
	mapTileRenderIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 13;
	mapTileRenderIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT] = 14;
	mapTileRenderIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 0;
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
			MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
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
			MsgAssert("nullptr GridActor Data를 참조하려 합니다.");
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
			MsgAssert("nullptr GridActor Data를 참조하려 합니다.");
			return;
		}

		Data.second->Move(false);
	}
}

float4 GridActor::GetScreenPos(const int2& _GridPos)
{
	return {
		((ContentConst::ACTOR_SIZE.x)* _GridPos.x) + ContentConst::ACTOR_SIZE.half().x,
		((ContentConst::ACTOR_SIZE.y)* _GridPos.y) + ContentConst::ACTOR_SIZE.half().y};
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
	InitRender({
		.FileName = "actor.BMP",
		.Pos = float4::Zero,
		.Scale = ContentConst::GRID_BITMAP_SIZE,
		.StartIndex = 0,
		.AnimLength = 4,
		.Order = 10,
		.BitmapInterval = 24});

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

	if (ACTOR_RENDER::TILE == RenderType)
	{
		SetTileRender();
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
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	vecBehaviors.clear();

	// Todo : File Save/Load 시스템이 완성된 후 데이터베이스 로드

	// 속성 값 초기화
	DefineData = 0;

	if (TEMP_ACTOR_TYPE::BABA == _Actor)
	{
		ActorName = "BABA";
		SetFrame(2);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;

		// Todo : 테스트용 임시 호출 추후 데이터시스템이 생성되면 삭제
		AddDefine(DEFINE_INFO::YOU);
		AddDefine(DEFINE_INFO::PUSH);
		AddDefine(DEFINE_INFO::HOT);
	}

	if (TEMP_ACTOR_TYPE::KEKE == _Actor)
	{
		ActorName = "KEKE";
		SetFrame(74);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;

		// Todo : 테스트용 임시 호출 추후 데이터시스템이 생성되면 삭제
		//AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::BABA_TEXT == _Actor)
	{
		ActorName = "BABA_TEXT";
		SetFrame(0);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::IS_TEXT == _Actor)
	{
		ActorName = "IS_TEXT";
		SetFrame(792);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::VERB_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::STOP);
	}

	if (TEMP_ACTOR_TYPE::YOU_TEXT == _Actor)
	{
		ActorName = "YOU_TEXT";
		SetFrame(864);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::LAVA == _Actor)
	{
		ActorName = "LAVA";
		SetFrame(434);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::TILE;
		AddDefine(DEFINE_INFO::MELT);
	}

	if (TEMP_ACTOR_TYPE::WATER == _Actor)
	{
		ActorName = "WATER";
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
		ActorName = "SKULL";
		SetFrame(722);
		SetLength(1);
		SetDirInterval(1);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::DYNAMIC;
		AddDefine(DEFINE_INFO::DEFEAT);
		AddDefine(DEFINE_INFO::PUSH);
	}

	if (TEMP_ACTOR_TYPE::WIN_TEXT == _Actor)
	{
		ActorName = "WIN_TEXT";
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
			MsgAssert("잘못된 Behavior Type 입니다.");
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
		MsgAssert("이미 죽은상태의 GridActor입니다.");
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

void GridActor::SetTileRender()
{
	int RenderKey = DIR_FLAG::NONE;
	int2 CheckPos = GridPos;

	CheckPos = GridPos + int2::Up;
	if (false == IsOver(CheckPos) && vecGridDatas[CheckPos.y][CheckPos.x].equals(ActorName))
	{
		RenderKey |= DIR_FLAG::UP;
	}

	CheckPos = GridPos + int2::Down;
	if (false == IsOver(CheckPos) && vecGridDatas[CheckPos.y][CheckPos.x].equals(ActorName))
	{
		RenderKey |= DIR_FLAG::DOWN;
	}

	CheckPos = GridPos + int2::Left;
	if (false == IsOver(CheckPos) && vecGridDatas[CheckPos.y][CheckPos.x].equals(ActorName))
	{
		RenderKey |= DIR_FLAG::LEFT;
	}

	CheckPos = GridPos + int2::Right;
	if (false == IsOver(CheckPos) && vecGridDatas[CheckPos.y][CheckPos.x].equals(ActorName))
	{
		RenderKey |= DIR_FLAG::RIGHT;
	}

	SetTileIndex(mapTileRenderIndex[RenderKey]);
}