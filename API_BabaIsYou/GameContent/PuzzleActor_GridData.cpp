#include "PuzzleActor.h"

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
