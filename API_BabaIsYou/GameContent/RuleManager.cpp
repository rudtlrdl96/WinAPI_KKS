#include "RuleManager.h"
#include "Rule.h"
#include "PuzzleActor.h"
#include "PuzzleActorManager.h"
#include "ContentDataBase.h"

RuleManager::RuleManager()
{
}

RuleManager::~RuleManager()
{
	for (const std::pair<int, Rule*>& Data : mapAllRules)
	{
		if (nullptr != Data.second)
		{
			delete Data.second;
		}
	}

	mapAllRules.clear();
}

void RuleManager::AddRule(Rule* _Rule)
{
	mapAllRules.insert({ _Rule->RuleKey, _Rule });
}

void RuleManager::RemoveRule(Rule* _Rule)
{
	std::map<int, Rule*>::iterator FindIter = mapAllRules.find(_Rule->RuleKey);

	if (FindIter != mapAllRules.end())
	{
		mapAllRules.erase(FindIter);
	}
}

void RuleManager::AddActorRule(PuzzleActor* _Actor, int _ActorIndex)
{
	for (const std::pair<int, Rule*>& Data : mapAllRules)
	{
		if (_ActorIndex == Data.second->SubjectActor->GetArrowDataIndex() && ACTOR_TYPE::DEFINE_TEXT == Data.second->DefineActor->GetActorType())
		{
			PuzzleActorManager::GetInst()->AddDefine(_Actor, Data.second->DefineActor->GetArrowDefine(), false);
		}
	}
}

int RuleManager::RuleCountCheck(int _SubjectDataIndex, int _VerbIndex, int _DefineDataIndex)
{
	int RuleCount = 0;

	for (const std::pair<int, Rule*>& Data : mapAllRules)
	{
		if (Data.second->SubjectActor->GetDataIndex() != _SubjectDataIndex)
		{
			continue;
		}

		if (Data.second->VerbActor->GetDataIndex() != _VerbIndex)
		{
			continue;
		}

		if (Data.second->DefineActor->GetDataIndex() != _DefineDataIndex)
		{
			continue;
		}

		++RuleCount;
	}

	return RuleCount;
}
