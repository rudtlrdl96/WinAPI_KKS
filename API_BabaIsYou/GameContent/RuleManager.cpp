#include "RuleManager.h"
#include "Rule.h"
#include "GridActor.h"

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

size_t RuleManager::GetActorRule(TEMP_ACTOR_INDEX _Actor)
{
	size_t CheckDefine = 0;

	for (const std::pair<int, Rule*>& Data : mapAllRules)
	{
		if (ACTOR_TYPE::DEFINE_TEXT == Data.second->DefineActor->GetActorType())
		{
			CheckDefine |= static_cast<size_t>(Data.second->DefineActor->GetArrowDefine());
		}
	}

	return CheckDefine;
}