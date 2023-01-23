#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "WiggleRender.h"
#include "Rule.h"
#include "RuleManager.h"
#include "GridActorManager.h"

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

		if ((false == _IsInputMove || false == Data.second->IsDefine(ACTOR_DEFINE::YOU)) &&
			true == Data.second->IsDefine(ACTOR_DEFINE::PUSH))
		{
			vecPushErase.push_back(Data.second);
		}
	}

	for (GridActor* Data : vecPushErase)
	{
		Data->SetDir(_Dir);
		Data->Push();
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
		if (LoopIter->second->IsDefine(ACTOR_DEFINE::YOU) && static_cast<size_t>(ACTOR_DEFINE::DEFEAT) & GridDefine)
		{
			LoopIter->second->ActorDeath();
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if (LoopIter->second->IsDefine(ACTOR_DEFINE::HOT) && static_cast<size_t>(ACTOR_DEFINE::MELT) & GridDefine)
		{
			LoopIter->second->ActorDeath();
			LoopIter = mapDatas.erase(LoopIter);
			continue;
		}
		else if ((true == SinkCheck || IsSinkValue) && static_cast<size_t>(ACTOR_DEFINE::SINK) & GridDefine)
		{
			Sink();
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
	size_t Info = static_cast<size_t>(ACTOR_DEFINE::NONE);

	for (const std::pair<int, GridActor*>& Data : mapDatas)
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

void GridActor::GridData::SinkCheckReset()
{
	IsSinkValue = false;
}

void GridActor::GridData::Sink()
{
	IsSinkValue = true;
}

#pragma endregion

/// static GridActor
#pragma region StaticFunc

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
size_t GridActor::ObjectPoolCount = 0;
bool GridActor::AnyActorMoveCheck = false;
bool GridActor::WinCheckValue = false;
int GridActor::NextActorKey = 0;
int2 GridActor::GridLength = int2::Zero;

std::vector<GridActor*> GridActor::vecObjectPool;
std::vector<std::vector<GridActor::GridData>> GridActor::vecGridDatas;
std::vector<std::vector<GridActor*>> GridActor::vecTextDatas;

std::map<TEMP_ACTOR_INDEX, std::map<int, GridActor*>> GridActor::mapActorDatas;
std::map<ACTOR_DEFINE, std::map<int, GridActor*>> GridActor::mapDefineActorDatas;
std::map<int, int> GridActor::mapTileRenderImageIndex;

GridActor* GridActor::CreateGridActor(TEMP_ACTOR_INDEX _Type)
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

GridActor* GridActor::GetTextActor(const int2& _Pos)
{
	return vecTextDatas[_Pos.y][_Pos.x];
}

std::map<int, GridActor*>& GridActor::GetActors(TEMP_ACTOR_INDEX _ActorIndex)
{
	return mapActorDatas[_ActorIndex];
}

std::map<int, GridActor*>& GridActor::GetActors(ACTOR_DEFINE _Define)
{
	return mapDefineActorDatas[_Define];
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
		PuzzleLevel->CreateActor<GridActor>();
	}

	mapTileRenderImageIndex[DIR_FLAG::NONE] = 0;
	mapTileRenderImageIndex[DIR_FLAG::RIGHT] = 1;
	mapTileRenderImageIndex[DIR_FLAG::UP] = 2;
	mapTileRenderImageIndex[DIR_FLAG::RIGHT | DIR_FLAG::UP] = 3;
	mapTileRenderImageIndex[DIR_FLAG::LEFT] = 4;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 5;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::UP] = 6;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::UP | DIR_FLAG::RIGHT] = 7;
	mapTileRenderImageIndex[DIR_FLAG::DOWN] = 8;
	mapTileRenderImageIndex[DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 9;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN] = 10;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 11;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN] = 12;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 13;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT] = 14;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 15;
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

void GridActor::DeleteGridActor()
{
	ResetGridActor();
	vecObjectPool.clear();
}

void GridActor::AllActorUndo()
{
	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		vecObjectPool[i]->Undo();
	}
}

void GridActor::AllActorRuleCheck()
{
	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		if (ACTOR_TYPE::ACTOR != vecObjectPool[i]->ActorType)
		{
			vecObjectPool[i]->RuleCheck();
		}
	}
}

void GridActor::GridActorEndCheck()
{
	for (size_t y = 0; y < vecGridDatas.size(); y++)
	{
		for (size_t x = 0; x < vecGridDatas[y].size(); x++)
		{
			vecGridDatas[y][x].DeathCheck();
			vecGridDatas[y][x].SinkCheckReset();
		}
	}

	for (size_t i = 0; i < ObjectPoolCount; i++)
	{
		vecObjectPool[i]->SaveBehaviorInfo();
	}
}

void GridActor::MoveAllYouBehavior(const int2& _Dir)
{
	for (const std::pair<int, GridActor*>& Data: mapDefineActorDatas[ACTOR_DEFINE::YOU])
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
	for (const std::pair<int, GridActor*>& Data : mapDefineActorDatas[ACTOR_DEFINE::MOVE])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr GridActor Data를 참조하려 합니다.");
			return;
		}

		Data.second->Move(false);
	}
}


void GridActor::SetGridLength(const int2& _Length)
{
	GridLength = _Length;
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
		_GridPos.x >= GridLength.x ||
		_GridPos.y >= GridLength.y)
	{
		return true;
	}

	return false;
}

bool GridActor::IsWin()
{
	return WinCheckValue;
}

bool GridActor::IsAnyMove()
{
	return AnyActorMoveCheck;
}

void GridActor::AnyMoveCheckReset()
{
	AnyActorMoveCheck = false;
}

#pragma endregion

/// static GridActor

GridActor::GridActor() :
	ActorKey(NextActorKey++)
{
	vecBehaviorBuffer.reserve(32);
	CurFramesBehaviorBuffer.reserve(4);
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

	if (ACTOR_RENDER_TYPE::TILE == RenderType)
	{
		SetTileRender();
	}

	if (false == IsDeath && IsDefine(ACTOR_DEFINE::YOU))
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

void GridActor::LoadData(TEMP_ACTOR_INDEX _Actor, bool _IsInit)
{
	if (_Actor < TEMP_ACTOR_INDEX::BABA || _Actor >= TEMP_ACTOR_INDEX::COUNT)
	{
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	{
		std::map<int, GridActor*>& PrevMapDatas = mapActorDatas[ActorEnum];

		std::map<int, GridActor*>::iterator FindIter = PrevMapDatas.find(ActorKey);
		std::map<int, GridActor*>::iterator EndIter = PrevMapDatas.end();

		if (FindIter != EndIter)
		{
			PrevMapDatas.erase(FindIter);
		}	
	}

	{
		std::map<ACTOR_DEFINE, std::map<int, GridActor*>>::iterator LoopIter = mapDefineActorDatas.begin();
		std::map<ACTOR_DEFINE, std::map<int, GridActor*>>::iterator EndIter = mapDefineActorDatas.end();

		for (; LoopIter != EndIter; ++LoopIter)
		{
			std::map<int, GridActor*>::iterator FindIter = LoopIter->second.find(ActorKey);

			if (FindIter != LoopIter->second.end())
			{
				GridActorManager::GetInst()->AddRemoveDefine(this, LoopIter->first);
			}
		}	
	}

	if (false == _IsInit)
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::CHANGE_INFO, static_cast<int>(ActorEnum) });
	}

	ActorEnum = _Actor;
	ArrowEnum = _Actor;

	mapActorDatas[ActorEnum][ActorKey] = this;
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

	// Todo : 테스트용 임시 호출 추후 데이터시스템이 생성되면 삭제
	if (TEMP_ACTOR_INDEX::BABA == _Actor)
	{
		ActorName = "BABA";
		WiggleRenderPtr->SetStartIndex(2);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::CHARACTER;
	}

	if (TEMP_ACTOR_INDEX::KEKE == _Actor)
	{
		ActorName = "KEKE";
		WiggleRenderPtr->SetStartIndex(74);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::CHARACTER;
	}

	if (TEMP_ACTOR_INDEX::FLAG == _Actor)
	{
		ActorName = "FLAG";
		WiggleRenderPtr->SetStartIndex(728);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
	}

	if (TEMP_ACTOR_INDEX::BABA_TEXT == _Actor)
	{
		ActorName = "BABA_TEXT";
		WiggleRenderPtr->SetStartIndex(0);
		ActorType = ACTOR_TYPE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowEnum = TEMP_ACTOR_INDEX::BABA;
	}

	if (TEMP_ACTOR_INDEX::KEKE_TEXT == _Actor)
	{
		ActorName = "KEKE_TEXT";
		WiggleRenderPtr->SetStartIndex(72);
		ActorType = ACTOR_TYPE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowEnum = TEMP_ACTOR_INDEX::KEKE;
	}

	if (TEMP_ACTOR_INDEX::IS_TEXT == _Actor)
	{
		ActorName = "IS_TEXT";
		WiggleRenderPtr->SetStartIndex(792);
		ActorType = ACTOR_TYPE::VERB_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
	}

	if (TEMP_ACTOR_INDEX::YOU_TEXT == _Actor)
	{
		ActorName = "YOU_TEXT";
		WiggleRenderPtr->SetStartIndex(864);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::YOU;
	}

	if (TEMP_ACTOR_INDEX::LAVA == _Actor)
	{
		ActorName = "LAVA";
		WiggleRenderPtr->SetStartIndex(434);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::TILE;
	}

	if (TEMP_ACTOR_INDEX::WATER == _Actor)
	{
		ActorName = "WATER";
		WiggleRenderPtr->SetStartIndex(362);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::TILE;
	}

	if (TEMP_ACTOR_INDEX::SKULL == _Actor)
	{
		ActorName = "SKULL";
		WiggleRenderPtr->SetStartIndex(722);
		ActorType = ACTOR_TYPE::ACTOR;
		RenderType = ACTOR_RENDER_TYPE::DYNAMIC;
	}

	if (TEMP_ACTOR_INDEX::WIN_TEXT == _Actor)
	{
		ActorName = "WIN_TEXT";
		WiggleRenderPtr->SetStartIndex(866);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::WIN;
	}

	if (TEMP_ACTOR_INDEX::STOP_TEXT == _Actor)
	{
		ActorName = "STOP_TEXT";
		WiggleRenderPtr->SetStartIndex(868);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::STOP;
	}

	if (TEMP_ACTOR_INDEX::PUSH_TEXT == _Actor)
	{
		ActorName = "PUSH_TEXT";
		WiggleRenderPtr->SetStartIndex(870);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::PUSH;
	}

	if (TEMP_ACTOR_INDEX::SINK_TEXT == _Actor)
	{
		ActorName = "SINK_TEXT";
		WiggleRenderPtr->SetStartIndex(872);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::SINK;
	}

	if (TEMP_ACTOR_INDEX::DEFEAT_TEXT == _Actor)
	{
		ActorName = "DEFEAT_TEXT";
		WiggleRenderPtr->SetStartIndex(874);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::DEFEAT;
	}

	if (TEMP_ACTOR_INDEX::HOT_TEXT == _Actor)
	{
		ActorName = "HOT_TEXT";
		WiggleRenderPtr->SetStartIndex(876);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::HOT;
	}

	if (TEMP_ACTOR_INDEX::MELT_TEXT == _Actor)
	{
		ActorName = "MELT_TEXT";
		WiggleRenderPtr->SetStartIndex(878);
		ActorType = ACTOR_TYPE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER_TYPE::STATIC;
		ArrowDefine = ACTOR_DEFINE::MELT;
	}

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		GetWiggleRender()->ActiveText();
		SetDefine(ACTOR_DEFINE::PUSH);
	}
	else
	{
		GetWiggleRender()->DisableText();
	}

	SetDefine(RuleManager::GetInst()->GetActorRule(_Actor));

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

	GetWiggleRender()->ActiveWiggle();
	GetWiggleRender()->SetAnimDir(int2::Right);
}

void GridActor::RuleCheck()
{
	Rule::CreateRule(this, true);
}

void GridActor::SetGrid(const int2& _Pos)
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

void GridActor::SetDefine(size_t _Info)
{
	mapDefineActorDatas[static_cast<ACTOR_DEFINE>(_Info)][ActorKey] = this;
	DefineData |= _Info;
}

void GridActor::SetDefine(ACTOR_DEFINE _Info)
{
	mapDefineActorDatas[_Info][ActorKey] = this;
	DefineData |= static_cast<size_t>(_Info);
}

void GridActor::AddDefine(ACTOR_DEFINE _Info)
{
	if (false == IsDefine(_Info))
	{
		mapDefineActorDatas[_Info][ActorKey] = this;
		CurFramesBehaviorBuffer.push_back({BEHAVIOR::DEFINE_ADD, static_cast<int>(_Info)});
	}

	DefineData |= static_cast<size_t>(_Info);
}

void GridActor::UndoAddDefine(ACTOR_DEFINE _Info)
{
	std::map<int, GridActor*>& mapDatas = mapDefineActorDatas[_Info];
	std::map<int, GridActor*>::iterator FindIter = mapDatas.find(ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}

	DefineData &= ~static_cast<size_t>(_Info);
}

void GridActor::RemoveDefine(ACTOR_DEFINE _Info)
{
	if (true == IsDefine(_Info))
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::DEFINE_REMOVE, static_cast<int>(_Info) });
	}

	std::map<int, GridActor*>& mapDatas = mapDefineActorDatas[_Info];
	std::map<int, GridActor*>::iterator FindIter = mapDatas.find(ActorKey);

	if (FindIter != mapDatas.end())
	{
		mapDatas.erase(FindIter);
	}

	DefineData &= ~static_cast<size_t>(_Info);
}

void GridActor::UndoRemoveDefine(ACTOR_DEFINE _Info)
{
	if (false == IsDefine(_Info))
	{
		mapDefineActorDatas[_Info][ActorKey] = this;
	}

	DefineData |= static_cast<size_t>(_Info);
}

bool GridActor::IsDefine(ACTOR_DEFINE _Info)
{
	return DefineData & static_cast<size_t>(_Info);
}

void GridActor::SaveBehaviorInfo()
{
	if (0 >= CurFramesBehaviorBuffer.size())
	{
		CurFramesBehaviorBuffer.push_back({ BEHAVIOR::WAIT, -1});
	}

	vecBehaviorBuffer.push_back(CurFramesBehaviorBuffer);
	CurFramesBehaviorBuffer.clear();
}

ACTOR_DEFINE GridActor::GetArrowDefine() const
{
	return ArrowDefine;
}

ACTOR_TYPE GridActor::GetActorType()
{
	return ActorType;
}

int2 GridActor::GetGridPos() const
{
	return GridPos;
}

void GridActor::Undo()
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
			LoadData(static_cast<TEMP_ACTOR_INDEX>(vecUndos[i].Value), true);
			break;
		default:
			MsgAssert("잘못된 Behavior Type 입니다.");
			break;
		}
	}

	vecBehaviorBuffer.pop_back();
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

	AnyActorMoveCheck = true;
	AllPushDir(MoveDir, _IsInputMove);
	
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();
		vecTextDatas[GridPos.y][GridPos.x] = nullptr;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos += MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		AddRule();
		vecTextDatas[GridPos.y][GridPos.x] = this;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::MOVE, -1});
	GetWiggleRender()->NextAnim();

	return true;
}

void GridActor::UndoMove()
{
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();
		vecTextDatas[GridPos.y][GridPos.x] = nullptr;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos -= MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		AddRule();
		vecTextDatas[GridPos.y][GridPos.x] = this;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	GetWiggleRender()->PrevAnim();
}

void GridActor::Push()
{
	if (true == IsDeath)
	{
		return;
	}
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();
		vecTextDatas[GridPos.y][GridPos.x] = nullptr;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos + MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		AddRule();
		vecTextDatas[GridPos.y][GridPos.x] = this;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

	CurFramesBehaviorBuffer.push_back({ BEHAVIOR::PUSH, -1});
}

void GridActor::UndoPush()
{
	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		RemoveRule();
		vecTextDatas[GridPos.y][GridPos.x] = nullptr;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = GridPos - MoveDir;
	MoveProgress = 0.0f;

	if (ACTOR_TYPE::ACTOR != ActorType)
	{
		AddRule();
		vecTextDatas[GridPos.y][GridPos.x] = this;
	}

	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	vecGridDatas[PrevPos.y][PrevPos.x].erase(this);

}

void GridActor::TurnLeft()
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

	GetWiggleRender()->SetAnimDir(MoveDir);
}

void GridActor::TurnRight()
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

	GetWiggleRender()->SetAnimDir(MoveDir);
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
	CurFramesBehaviorBuffer.push_back({BEHAVIOR::DEATH, -1});

	GetWiggleRender()->RenderOff();
}

void GridActor::UndoActorDeath()
{
	vecGridDatas[GridPos.y][GridPos.x].push_back(this);
	IsDeath = false;
	GetWiggleRender()->RenderOn();
}

void GridActor::AllPushDir(const int2& _Dir, bool _IsInputMove)
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

void GridActor::WinCheck()
{
	size_t DefineInfo = vecGridDatas[GridPos.y][GridPos.x].GetDefine();

	if (DefineInfo & static_cast<size_t>(ACTOR_DEFINE::WIN))
	{
		WinCheckValue = true;
	}
}


void GridActor::AddRule()
{
	Rule::CreateRule(this);
}

void GridActor::RemoveRule()
{
	Rule::RemoveRule(this);
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

	GetWiggleRender()->SetTileIndex(mapTileRenderImageIndex[RenderKey]);
}

void GridActor::ResetValues()
{
	vecBehaviorBuffer.clear();
	CurFramesBehaviorBuffer.clear();

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
