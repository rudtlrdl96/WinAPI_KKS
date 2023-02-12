#include "PuzzleActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "SoundSystem.h"


/// static GridActor
#pragma region StaticFunc

GameEngineLevel* PuzzleActor::PuzzleLevel = nullptr;
size_t PuzzleActor::ObjectPoolCount = 0;
bool PuzzleActor::AnyActorMoveCheck = false;
bool PuzzleActor::AnyActorUndoCheck = false;
bool PuzzleActor::AnyRuleCreateCheck = false;
bool PuzzleActor::WinCheckValue = false;
int PuzzleActor::YouActorCount = 0;
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

	if (true == AnyActorUndoCheck)
	{
		SoundSystem::GetLevelSoundSystem()->EffectPlay("Undo_");
	}

	if (true == AnyRuleCreateCheck)
	{
		SoundSystem::GetLevelSoundSystem()->EffectPlay("TextCompletion_");
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
	for (const std::pair<int, PuzzleActor*>& Data : mapDefineActorDatas[ACTOR_DEFINE::YOU])
	{
		if (nullptr == Data.second)
		{
			MsgAssert("nullptr PuzzleActor Data를 참조하려 합니다.");
			return;
		}

		Data.second->SetDir(_Dir);
		Data.second->Move(true);
	}

	if (true == AnyActorMoveCheck)
	{
		SoundSystem::GetLevelSoundSystem()->EffectPlay("Move_");
	}

	if (true == AnyRuleCreateCheck)
	{
		SoundSystem::GetLevelSoundSystem()->EffectPlay("TextCompletion_");
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
		((ContentConst::ACTOR_SIZE.x) * _GridPos.x) + ContentConst::ACTOR_SIZE.half().x,
		((ContentConst::ACTOR_SIZE.y) * _GridPos.y) + ContentConst::ACTOR_SIZE.half().y };
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

void PuzzleActor::AnyCheckReset()
{
	AnyActorMoveCheck = false;
	AnyActorUndoCheck = false;
	AnyRuleCreateCheck = false;
}

int PuzzleActor::GetYouPuzzleActorCount()
{
	return YouActorCount;
}

void PuzzleActor::ResetYouActorCount()
{
	YouActorCount = 0;
}

#pragma endregion
