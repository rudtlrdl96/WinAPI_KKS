#include "PuzzleActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "WiggleRender.h"
#include "Rule.h"
#include "RuleManager.h"
#include "PuzzleActorManager.h"
#include "ContentDataBase.h"
#include "ParticleSystem.h"
#include "ContentRand.h"
#include "ParticleActor.h"
#include "CameraSystem.h"

/// GridData

#pragma region GridData

void PuzzleActor::GridData::push_back(PuzzleActor* _Actor)
{
	mapDatas[_Actor->ActorKey] = _Actor;
}

void PuzzleActor::GridData::erase(PuzzleActor* _Actor)
{
	std::map<int, PuzzleActor*>::iterator FindIter = mapDatas.find(_Actor->ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}
}

void PuzzleActor::GridData::clear()
{
	mapDatas.clear();
}

void PuzzleActor::GridData::Push(const int2& _Pos, const int2& _Dir, bool _IsInputMove)
{
	if (0 >= mapDatas.size())
	{
		return;
	}

	std::vector<PuzzleActor*> vecPushErase;
	vecPushErase.reserve(mapDatas.size());

	int2 NextPos = _Pos + _Dir;

	for (const std::pair<int, PuzzleActor*>& Data : mapDatas)
	{
		if (true == Data.second->IsDeath)
		{
			continue;
		}

		if ((false == _IsInputMove || false == Data.second->IsDefine(ACTOR_DEFINE::YOU)) &&
			true == Data.second->IsDefine(ACTOR_DEFINE::PUSH))
		{
			vecPushErase.push_back(Data.second);
		}
	}

	for (PuzzleActor* Data : vecPushErase)
	{
		Data->SetDir(_Dir);
		Data->Push();
	}
}


void PuzzleActor::GridData::DeathCheck()
{
	size_t GridDefine = GetDefine();

	std::map<int, PuzzleActor*>::iterator LoopIter = mapDatas.begin();
	std::map<int, PuzzleActor*>::iterator EndIter = mapDatas.end();

	bool SinkCheck = 1 < mapDatas.size();

	while (LoopIter != EndIter)
	{ 
		if (LoopIter->second->IsDefine(ACTOR_DEFINE::YOU) && static_cast<size_t>(ACTOR_DEFINE::DEFEAT) & GridDefine)
		{
			LoopIter->second->ActorDeath(ACTOR_DEFINE::DEFEAT);
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if (LoopIter->second->IsDefine(ACTOR_DEFINE::MELT) && static_cast<size_t>(ACTOR_DEFINE::HOT) & GridDefine)
		{
			LoopIter->second->ActorDeath(ACTOR_DEFINE::HOT);
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if ((true == SinkCheck || IsSinkValue) && static_cast<size_t>(ACTOR_DEFINE::SINK) & GridDefine)
		{
			Sink();
			LoopIter->second->ActorDeath(ACTOR_DEFINE::SINK);
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}

		++LoopIter;
	}
}

bool PuzzleActor::GridData::equals(const std::string_view& _Name)
{
	for (const std::pair<int, PuzzleActor*>& Data : mapDatas)
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

size_t PuzzleActor::GridData::GetDefine()
{
	size_t Info = static_cast<size_t>(ACTOR_DEFINE::NONE);

	for (const std::pair<int, PuzzleActor*>& Data : mapDatas)
	{
		if (true == Data.second->IsDeath)
		{
			continue;
		}

		Info |= Data.second->GetDefine();
	}

	if (true == IsSinkValue)
	{
		Info |= static_cast<size_t>(ACTOR_DEFINE::SINK);
	}

	return Info;
}

void PuzzleActor::GridData::SinkCheckReset()
{
	IsSinkValue = false;
}

void PuzzleActor::GridData::Sink()
{
	IsSinkValue = true;
}

#pragma endregion

/// static GridActor
#pragma region StaticFunc

GameEngineLevel* PuzzleActor::PuzzleLevel = nullptr;
size_t PuzzleActor::ObjectPoolCount = 0;
bool PuzzleActor::AnyActorMoveCheck = false;
bool PuzzleActor::WinCheckValue = false;
int PuzzleActor::NextActorKey = 0;
int2 PuzzleActor::GridLength = int2::Zero;

std::vector<PuzzleActor*> PuzzleActor::vecObjectPool;
std::vector<std::vector<PuzzleActor::GridData>> PuzzleActor::vecGridDatas;
std::vector<std::vector<PuzzleActor*>> PuzzleActor::vecTextDatas;

std::map<int, std::map<int, PuzzleActor*>> PuzzleActor::mapActorDatas;
std::map<ACTOR_DEFINE, std::map<int, PuzzleActor*>> PuzzleActor::mapDefineActorDatas;

PuzzleActor* PuzzleActor::CreatePuzzlActor(int _Type)
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

PuzzleActor* PuzzleActor::GetTextActor(const int2& _Pos)
{
	if (0 > _Pos.x || ContentConst::GRID_SIZE_X <= _Pos.x)
	{
		return nullptr;
	}

	if (0 > _Pos.y || ContentConst::GRID_SIZE_Y <= _Pos.y)
	{
		return nullptr;
	}

	return vecTextDatas[_Pos.y][_Pos.x];
}

std::map<int, PuzzleActor*>& PuzzleActor::GetActors(int _ActorIndex)
{
	return mapActorDatas[_ActorIndex];
}

std::map<int, PuzzleActor*>& PuzzleActor::GetActors(ACTOR_DEFINE _Define)
{
	return mapDefineActorDatas[_Define];
}


void PuzzleActor::InitGridActor(GameEngineLevel* _PuzzleLevel)
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
	vecTextDatas.resize(ContentConst::GRID_SIZE.y);

	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		vecGridDatas[y].resize(ContentConst::GRID_SIZE.x);
	}

	for (size_t y = 0; y < vecTextDatas.size(); y++)
	{
		vecTextDatas[y].resize(ContentConst::GRID_SIZE.x);

		for (size_t x = 0; x < vecTextDatas[y].size(); x++)
		{
			vecTextDatas[y][x] = nullptr;
		}
	}

	PuzzleLevel = _PuzzleLevel;

	vecObjectPool.reserve(ContentConst::GRID_SIZE.x * ContentConst::GRID_SIZE.y);

	for (size_t i = 0; i < vecObjectPool.capacity(); i++)
	{
		PuzzleLevel->CreateActor<PuzzleActor>();
	}
}


void PuzzleActor::ResetPuzzleActor()
{
	for (size_t i = 0; i < vecObjectPool.size(); i++)
	{
		if (nullptr == vecObjectPool[i])
		{
			MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
			return;
		}

		vecObjectPool[i]->ResetValues();
		vecObjectPool[i]->Off();
	}

	for (size_t y = 0; y < vecTextDatas.size(); y++)
	{
		for (size_t x = 0; x < vecTextDatas[y].size(); x++)
		{
			vecTextDatas[y][x] = nullptr;
		}
	}

	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		for (size_t x = 0; x < vecGridDatas[y].size(); x++)
		{
			vecGridDatas[y][x].clear();
		}
	}

	mapActorDatas.clear();
	mapDefineActorDatas.clear();
	ObjectPoolCount = 0;
	WinCheckValue = false;
	AnyActorMoveCheck = false;
}

void PuzzleActor::DeletePuzzleActor()
{
	ResetPuzzleActor();
	vecObjectPool.clear();
}

void PuzzleActor::AllActorUndo()
{
	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		vecObjectPool[i]->Undo();
	}
}

void PuzzleActor::AllActorRuleCheck()
{
	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		if (ACTOR_TYPE::ACTOR != vecObjectPool[i]->ActorType)
		{
			vecObjectPool[i]->RuleCheck();
		}
	}
}


void PuzzleActor::PuzzleActorDeathCheck()
{
	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		for (size_t x = 0; x < vecGridDatas[y].size(); x++)
		{
			vecGridDatas[y][x].DeathCheck();
			vecGridDatas[y][x].SinkCheckReset();
		}
	}
}

void PuzzleActor::PuzzleActorSaveBehavior()
{
	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		vecObjectPool[i]->SaveBehaviorInfo();
	}
}

void PuzzleActor::MoveAllYouBehavior(const int2& _Dir)
{
	for (const std::pair<int, PuzzleActor*>& Data: mapDefineActorDatas[ACTOR_DEFINE::YOU])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr PuzzleActor Data를 참조하려 합니다.");
			return;
		}

		Data.second->SetDir(_Dir);
		Data.second->Move(true);
	}
}

void PuzzleActor::MoveAllMoveBehavior()
{
	for (const std::pair<int, PuzzleActor*>& Data : mapDefineActorDatas[ACTOR_DEFINE::MOVE])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr PuzzleActor Data를 참조하려 합니다.");
			return;
		}

		Data.second->Move(false);
	}
}


void PuzzleActor::SetGridLength(const int2& _Length)
{
	GridLength = _Length;
}

float4 PuzzleActor::GetScreenPos(const int2& _GridPos)
{
	return {
		((ContentConst::ACTOR_SIZE.x)* _GridPos.x) + ContentConst::ACTOR_SIZE.half().x,
		((ContentConst::ACTOR_SIZE.y)* _GridPos.y) + ContentConst::ACTOR_SIZE.half().y};
}


bool PuzzleActor::IsOver(const int2& _GridPos)
{
	if (_GridPos.x < 0 ||
		_GridPos.y < 0 ||
		_GridPos.x >= GridLength.x ||
		_GridPos.y >= GridLength.y)
	{
		return true;
	}

	return false;
}

bool PuzzleActor::IsWin()
{
	return WinCheckValue;
}

bool PuzzleActor::IsAnyMove()
{
	return AnyActorMoveCheck;
}

void PuzzleActor::AnyMoveCheckReset()
{
	AnyActorMoveCheck = false;
}

#pragma endregion

/// static GridActor

PuzzleActor::PuzzleActor() :
	ActorKey(NextActorKey++)
{
	vecBehaviorBuffer.reserve(32);
	CurFramesBehaviorBuffer.reserve(4);
}

PuzzleActor::~PuzzleActor()
{
}

void PuzzleActor::Start()
{
	InitWiggleRender({
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

void PuzzleActor::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (true == IsOver(GridPos))
	{
		MsgAssert("액터가 그리드 밖으로 벗어났습니다.");
		return;
	}

	if (false == IsDeath)
	{
		ActorParticleCreate(_DT);
	}

	if (ACTOR_RENDER_TYPE::TILE == RenderType)
	{
		SetTileRender();
	}

	if (false == WinCheckValue && false == IsDeath && IsDefine(ACTOR_DEFINE::YOU))
	{
		WinCheck();
	}

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		if (0 < mapRules.size())
		{
			GetWiggleRender()->SetTextIndex(0);
		}
		else
		{
			GetWiggleRender()->SetTextIndex(1);
		}
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

void PuzzleActor::LoadData(int _Actor, int2 _Dir, bool _IsInit)
{
	const ActorData* LoadDB = ContentDataBase::GetInst()->GetData(_Actor);

	if (nullptr == LoadDB)
	{
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	{
		std::map<int, PuzzleActor*>& PrevMapDatas = mapActorDatas[ActorEnum];

		std::map<int, PuzzleActor*>::iterator FindIter = PrevMapDatas.find(ActorKey);
		std::map<int, PuzzleActor*>::iterator EndIter = PrevMapDatas.end();

		if (FindIter != EndIter)
		{
			PrevMapDatas.erase(FindIter);
		}	
	}

	{
		std::map<ACTOR_DEFINE, std::map<int, PuzzleActor*>>::iterator LoopIter = mapDefineActorDatas.begin();
		std::map<ACTOR_DEFINE, std::map<int, PuzzleActor*>>::iterator EndIter = mapDefineActorDatas.end();

		for (; LoopIter != EndIter; ++LoopIter)
		{
			std::map<int, PuzzleActor*>::iterator FindIter = LoopIter->second.find(ActorKey);

			if (FindIter != LoopIter->second.end())
			{
				PuzzleActorManager::GetInst()->AddDefine(this, LoopIter->first, true);
			}
		}	
	}

	if (false == _IsInit)
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::CHANGE_INFO, static_cast<int>(ActorEnum) });
	}

	DefineData = 0;

	// Todo : File Save/Load 시스템이 완성된 후 데이터베이스 로드

	// 속성 값 초기화
	WiggleRender* WiggleRenderPtr = GetWiggleRender();

	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender가 초기화되지 않았습니다.");
		return;
	}

	WiggleRenderPtr->Reset();

	ActorEnum = LoadDB->ActorEnum;
	ArrowEnum = LoadDB->ArrowEnum;
	ActorName = LoadDB->ActorName;
	ActorType = LoadDB->ActorType;
	RenderType = LoadDB->RenderType;
	ArrowDefine = LoadDB->ArrowDefine;
	ActorColor = LoadDB->Color;
	MoveDir = _Dir;

	mapActorDatas[ActorEnum][ActorKey] = this;


	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		GetWiggleRender()->ActiveText();
		SetDefine(ACTOR_DEFINE::PUSH);
	}
	else
	{
		GetWiggleRender()->DisableText();
	}

	RuleManager::GetInst()->AddActorRule(this, _Actor);

	WiggleRenderPtr->SetRender(CreateRender("Actor.BMP", static_cast<int>(RENDER_ORDER::ACTORSTART) + LoadDB->RenderOrder));
	WiggleRenderPtr->SetStartIndex(LoadDB->RenderIndex);

	if (true == IsDeath)
	{
		WiggleRenderPtr->RenderOff();
	}

	switch (RenderType)
	{
	case ACTOR_RENDER_TYPE::STATIC:
		WiggleRenderPtr->SetAnimLength(1);
		WiggleRenderPtr->SetDirInterval(0);
		WiggleRenderPtr->DisableAnimation();
		WiggleRenderPtr->DisableTile();
		WiggleRenderPtr->DisableDir();
		break;
	case ACTOR_RENDER_TYPE::DYNAMIC:
		WiggleRenderPtr->SetAnimLength(1);
		WiggleRenderPtr->SetDirInterval(1);
		WiggleRenderPtr->DisableAnimation();
		WiggleRenderPtr->DisableTile();
		WiggleRenderPtr->ActiveDir();
		break;
	case ACTOR_RENDER_TYPE::CHARACTER:
		WiggleRenderPtr->SetAnimLength(4);
		WiggleRenderPtr->SetDirInterval(4);
		WiggleRenderPtr->ActiveAnimation();
		WiggleRenderPtr->DisableTile();
		WiggleRenderPtr->ActiveDir();
		break;
	case ACTOR_RENDER_TYPE::TILE:
		WiggleRenderPtr->SetAnimLength(1);
		WiggleRenderPtr->SetDirInterval(0);
		WiggleRenderPtr->DisableAnimation();
		WiggleRenderPtr->ActiveTile();
		WiggleRenderPtr->DisableDir();
		break;
	case ACTOR_RENDER_TYPE::BELT:
		WiggleRenderPtr->SetAnimLength(4);
		WiggleRenderPtr->SetDirInterval(4);
		WiggleRenderPtr->ActiveAnimation();
		WiggleRenderPtr->ActiveTile();
		WiggleRenderPtr->DisableDir();
		break;
	default:
		break;
	}
	GetWiggleRender()->SetAnimDir(MoveDir);
	GetWiggleRender()->ActiveWiggle();
}

void PuzzleActor::LoadData(int _Actor, DIR_FLAG _Dir, bool _IsInit)
{
	int2 InitDir = int2::Zero;

	if (DIR_FLAG::UP == _Dir)
	{
		InitDir = int2::Up;
	}
	else if (DIR_FLAG::DOWN == _Dir)
	{
		InitDir = int2::Down;
	}
	else if (DIR_FLAG::LEFT == _Dir)
	{
		InitDir = int2::Left;
	}
	else if (DIR_FLAG::RIGHT == _Dir)
	{
		InitDir = int2::Right;
	}
	else
	{
		MsgAssert("잘못된 방향으로 PuzzleActor를 초기화하려 했습니다");
		return;
	}

	LoadData(_Actor, InitDir, _IsInit);
}

void PuzzleActor::RuleCheck()
{
	Rule::CreateRule(this, true);
}

void PuzzleActor::SetGrid(const int2& _Pos)
{
	if (false == IsOver(GridPos))
	{
		vecGridDatas[GridPos.y][GridPos.x].erase(this);
	}

	GridPos = _Pos;
	vecGridDatas[GridPos.y][GridPos.x].push_back(this);

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		vecTextDatas[GridPos.y][GridPos.x] = this;
	}

	SetPos(GetScreenPos(GridPos));
}

void PuzzleActor::SetDefine(size_t _Info)
{
	mapDefineActorDatas[static_cast<ACTOR_DEFINE>(_Info)][ActorKey] = this;
	DefineData |= _Info;
}

void PuzzleActor::SetDefine(ACTOR_DEFINE _Info)
{
	mapDefineActorDatas[_Info][ActorKey] = this;
	DefineData |= static_cast<size_t>(_Info);
}

void PuzzleActor::AddDefine(ACTOR_DEFINE _Info)
{
	if (false == IsDefine(_Info))
	{
		mapDefineActorDatas[_Info][ActorKey] = this;
		CurFramesBehaviorBuffer.push_back({BEHAVIOR::DEFINE_ADD, static_cast<int>(_Info)});
	}

	DefineData |= static_cast<size_t>(_Info);
}

void PuzzleActor::UndoAddDefine(ACTOR_DEFINE _Info)
{
	std::map<int, PuzzleActor*>& mapDatas = mapDefineActorDatas[_Info];
	std::map<int, PuzzleActor*>::iterator FindIter = mapDatas.find(ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}

	DefineData &= ~static_cast<size_t>(_Info);
}

void PuzzleActor::RemoveDefine(ACTOR_DEFINE _Info)
{
	if (true == IsDefine(_Info))
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::DEFINE_REMOVE, static_cast<int>(_Info) });
	}

	std::map<int, PuzzleActor*>& mapDatas = mapDefineActorDatas[_Info];
	std::map<int, PuzzleActor*>::iterator FindIter = mapDatas.find(ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}

	DefineData &= ~static_cast<size_t>(_Info);
}

void PuzzleActor::UndoRemoveDefine(ACTOR_DEFINE _Info)
{
	if (false == IsDefine(_Info))
	{
		mapDefineActorDatas[_Info][ActorKey] = this;
	}

	DefineData |= static_cast<size_t>(_Info);
}

bool PuzzleActor::IsDefine(ACTOR_DEFINE _Info)
{
	return DefineData & static_cast<size_t>(_Info);
}

void PuzzleActor::SaveBehaviorInfo()
{
	if (0 >= CurFramesBehaviorBuffer.size())
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::WAIT, -1});
	}

	vecBehaviorBuffer.push_back(CurFramesBehaviorBuffer);
	CurFramesBehaviorBuffer.clear();
}

int PuzzleActor::GetArrowEnum() const
{
	return ArrowEnum;
}

ACTOR_DEFINE PuzzleActor::GetArrowDefine() const
{
	return ArrowDefine;
}

ACTOR_TYPE PuzzleActor::GetActorType() const
{
	return ActorType;
}

int2 PuzzleActor::GetGridPos() const
{
	return GridPos;
}

int2 PuzzleActor::GetMoveDir() const
{
	return MoveDir;
}

void PuzzleActor::Undo()
{
	if (0 >= vecBehaviorBuffer.size())
	{
		return;
	}

	const std::vector<BehavoirData>& vecUndos = vecBehaviorBuffer.back();

	for (int i = static_cast<int>(vecUndos.size() - 1); i >= 0 ; --i)
	{
		switch (vecUndos[i].Behavior)
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
		case BEHAVIOR::DEFINE_ADD:
			UndoAddDefine(static_cast<ACTOR_DEFINE>(vecUndos[i].Value));
			break;
		case BEHAVIOR::DEFINE_REMOVE:
			UndoRemoveDefine(static_cast<ACTOR_DEFINE>(vecUndos[i].Value));
			break;
		case BEHAVIOR::CHANGE_INFO:
			LoadData(vecUndos[i].Value, MoveDir, true);
			break;
		default:
			MsgAssert("잘못된 Behavior Type 입니다.");
			break;
		}
	}

	vecBehaviorBuffer.pop_back();
}

bool PuzzleActor::Move(bool _IsInputMove)
{
	if (true == IsDeath)
	{
		return false;
	}

	if (false == CanMove(GridPos + MoveDir))
	{
		return false;
	}

	AnyActorMoveCheck = true;
	AllPushDir(MoveDir, _IsInputMove);
	
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();

		if (this == vecTextDatas[GridPos.y][GridPos.x])
		{
			vecTextDatas[GridPos.y][GridPos.x] = nullptr;
		}
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos += MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		vecTextDatas[GridPos.y][GridPos.x] = this;
		AddRule();
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::MOVE, -1});
	GetWiggleRender()->NextAnim();

	float4 MoveParticlePos = GetPos();

	if (int2::Left == MoveDir || int2::Right == MoveDir)
	{
		MoveParticlePos += {0, 5};
	}

	ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle("Move", ActorColor, MoveParticlePos, {25, 25});
	ParticlePtr->MoveParticle(-MoveDir.ToFloat4(), 50.0f);

	return true;
}

void PuzzleActor::UndoMove()
{
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();

		if (this == vecTextDatas[GridPos.y][GridPos.x])
		{
			vecTextDatas[GridPos.y][GridPos.x] = nullptr;
		}
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos -= MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		vecTextDatas[GridPos.y][GridPos.x] = this;
		AddRule();
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	GetWiggleRender()->PrevAnim();
}

void PuzzleActor::Push()
{
	if (true == IsDeath)
	{
		return;
	}
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();

		if (this == vecTextDatas[GridPos.y][GridPos.x])
		{
			vecTextDatas[GridPos.y][GridPos.x] = nullptr;
		}
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos + MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		vecTextDatas[GridPos.y][GridPos.x] = this;
		AddRule();
	}

	float4 MoveParticlePos = GetPos();

	if (int2::Left == MoveDir || int2::Right == MoveDir)
	{
		MoveParticlePos += {0, 5};
	}

	ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle("Move", ActorColor, MoveParticlePos, { 25, 25 });
	ParticlePtr->MoveParticle(-MoveDir.ToFloat4(), 50.0f);

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::PUSH, -1});
}

void PuzzleActor::UndoPush()
{
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();

		if (this == vecTextDatas[GridPos.y][GridPos.x])
		{
			vecTextDatas[GridPos.y][GridPos.x] = nullptr;
		}
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos - MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		vecTextDatas[GridPos.y][GridPos.x] = this;
		AddRule();
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

}

void PuzzleActor::TurnLeft()
{
	if (true == IsDeath)
	{
		return;
	}

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::TURN_LEFT, -1 });

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

	GetWiggleRender()->SetAnimDir(MoveDir);
}

void PuzzleActor::UndoTurnLeft()
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

	GetWiggleRender()->SetAnimDir(MoveDir);
}

void PuzzleActor::TurnRight()
{
	if (true == IsDeath)
	{
		return;
	}

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::TURN_RIGHT, -1});

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

	GetWiggleRender()->SetAnimDir(MoveDir);
}

void PuzzleActor::UndoTurnRight()
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

	GetWiggleRender()->SetAnimDir(MoveDir);
}

void PuzzleActor::SetDir(const int2& _Dir)
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

void PuzzleActor::ActorDeath(ACTOR_DEFINE _DeathCause)
{
	if (true == IsDeath)
	{
		MsgAssert("이미 죽은상태의 PuzzleActor입니다.");
		return;
	}

	IsDeath = true;
	CurFramesBehaviorBuffer.push_back({BEHAVIOR::DEATH, -1});

	int ParticleCount = 0;
	std::string DeathParticleName = "";
	PARTICLE_COLOR ParticleColor = PARTICLE_COLOR::WHITE;

	switch (_DeathCause)
	{
	case ACTOR_DEFINE::HOT:
	{
		ParticleCount = 5;
		DeathParticleName = "Smoke";
		ParticleColor = PARTICLE_COLOR::GRAY;
	}
		break;
	case ACTOR_DEFINE::SINK:
	{
		ParticleCount = 8;
		DeathParticleName = "Explosion";
		ParticleColor = PARTICLE_COLOR::JAVA;
	}
		break;
	case ACTOR_DEFINE::DEFEAT:
	{
		ParticleCount = 5;
		DeathParticleName = "Explosion";
	}
		break;
	}

	float4 DeathGridPos = GetScreenPos(GetGridPos());

	for (size_t i = 0; i < ParticleCount; i++)
	{
		float4 RandPos = float4::Zero;
		float RandRot = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);
		RandPos.x = std::cos(RandRot);
		RandPos.y = std::sin(RandRot);
		RandPos *= ContentRand::RandFloat(15.0f, 30.0f);

		RandPos += DeathGridPos;

		ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle(DeathParticleName, ParticleColor, RandPos, { 30.0f, 30.0f });
		ParticlePtr->MoveParticle((RandPos - DeathGridPos), ContentRand::RandFloat(40.0f, 60.0f));
	}

	CameraSystem::GetLevelCameraSystem()->CameraShake(7.0f, 40.0f , 4);
	GetWiggleRender()->RenderOff();
}

void PuzzleActor::UndoActorDeath()
{
	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	IsDeath = false;
	GetWiggleRender()->RenderOn();
}

void PuzzleActor::AllPushDir(const int2& _Dir, bool _IsInputMove)
{
	int2 PushPos = GridPos + _Dir;

	while (false == IsOver(PushPos))
	{
		size_t DefineData = vecGridDatas[PushPos.y][PushPos.x].GetDefine();

		if (static_cast<size_t>(ACTOR_DEFINE::YOU) & DefineData)
		{
			break;
		}

		if (static_cast<size_t>(ACTOR_DEFINE::PUSH) & DefineData)
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

bool PuzzleActor::CanMove(const int2& _NextPos)
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

		if (static_cast<size_t>(ACTOR_DEFINE::STOP) & DefineData)
		{
			return false;
		}

		if (static_cast<size_t>(ACTOR_DEFINE::PUSH) & DefineData)
		{
			CheckPos += Dir;
			continue;
		}

		if (static_cast<size_t>(ACTOR_DEFINE::YOU) & DefineData)
		{
			return true;
		}

		break;
	}

	return true;
}

void PuzzleActor::WinCheck()
{
	size_t DefineInfo = vecGridDatas[GridPos.y][GridPos.x].GetDefine();

	if (DefineInfo & static_cast<size_t>(ACTOR_DEFINE::WIN))
	{
		float4 DeathGridPos = GetScreenPos(GetGridPos());

		for (size_t i = 0; i < 10; i++)
		{
			float4 RandPos = float4::Zero;
			float RandRot = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);
			RandPos.x = std::cos(RandRot);
			RandPos.y = std::sin(RandRot);
			RandPos *= ContentRand::RandFloat(5.0f, 25.0f);

			RandPos += DeathGridPos;

			ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle("Win", PARTICLE_COLOR::FLAX, RandPos, { 30.0f, 30.0f });
			ParticlePtr->MoveParticle((RandPos - DeathGridPos), ContentRand::RandFloat(50.0f, 110.0f));
		}

		WinCheckValue = true;
	}
}


void PuzzleActor::AddRule()
{
	Rule::CreateRule(this);
}

void PuzzleActor::RemoveRule()
{
	Rule::RemoveRule(this);
}

void PuzzleActor::SetTileRender()
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

	GetWiggleRender()->SetTileIndex(ContentConst::GetTile(RenderKey));
}

void PuzzleActor::ActorParticleCreate(float _DT)
{
	if (false == (IsDefine(ACTOR_DEFINE::WIN) || IsDefine(ACTOR_DEFINE::HOT)))
	{
		return;
	}

	ParticleTime += _DT;

	if (ParticleTime >= NextParticleTime)
	{

		ParticleTime = 0.0f;

		float4 RandPos = float4::Zero;
		float RandRot = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);
		float RandMoveDis = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);

		RandPos.x = std::cos(RandRot);
		RandPos.y = std::sin(RandRot);
		RandPos *= ContentRand::RandFloat(10.0f, 35.0f);

		RandPos += GetPos();

		if (IsDefine(ACTOR_DEFINE::WIN))
		{
			ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle("Glittering", PARTICLE_COLOR::FLAX, RandPos, {30.0f, 30.0f});
			ParticlePtr->MoveParticle(RandPos - GetPos(), ContentRand::RandFloat(10.0f, 30.0f));
			NextParticleTime = ContentRand::RandFloat(0.25f, 0.8f);
		}
		else if (IsDefine(ACTOR_DEFINE::HOT))
		{
			ParticleActor* ParticlePtr = ParticleSystem::GetLevelParticleSystem()->UseParticle("Smoke", PARTICLE_COLOR::GRAY, RandPos, { 30.0f, 30.0f });
			ParticlePtr->MoveParticle(RandPos - GetPos(), ContentRand::RandFloat(5.0f, 15.0f));
			NextParticleTime = ContentRand::RandFloat(5.0f, 50.0f);
		}
	}
}

void PuzzleActor::ResetValues()
{
	vecBehaviorBuffer.clear();
	CurFramesBehaviorBuffer.clear();
	RemoveRule();

	MoveDir = int2::Right;
	PrevPos = { -1, -1 };
	GridPos = int2::Zero;

	DefineData = 0;

	IsDeath = false;
	IsMove = false;
	MoveProgress = 0.0f;

	GetWiggleRender()->ResetAnim();
	GetWiggleRender()->RenderOn();
}
