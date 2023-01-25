#include "RuleManager.h"
#include "Rule.h"
#include "GridActor.h"
#include "GridActorManager.h"

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

void RuleManager::AddActorRule(GridActor* _Actor, TEMP_ACTOR_INDEX _ActorIndex)
{
	for (const std::pair<int, Rule*>& Data : mapAllRules)
	{
		if (_ActorIndex == Data.second->SubjectActor->GetArrowEnum() && ACTOR_TYPE::DEFINE_TEXT == Data.second->DefineActor->GetActorType())
		{
			GridActorManager::GetInst()->AddDefine(_Actor, Data.second->DefineActor->GetArrowDefine(), false);
		}
	}
}