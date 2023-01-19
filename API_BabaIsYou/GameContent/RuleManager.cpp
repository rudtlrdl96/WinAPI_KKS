#include "RuleManager.h"
#include <GameEngineBase/GameEngineDebug.h>

RuleManager::RuleManager()
{
}

RuleManager::~RuleManager()
{
}

void RuleManager::AddRule(const Rule& _Rule)
{
	std::map<int, Rule>::iterator FindIter = mapRules.find(_Rule.Value.KeyValue);
	std::map<int, Rule>::iterator EndIter = mapRules.end();

	if (FindIter == EndIter)
	{
		mapRules[_Rule.Value.KeyValue] = _Rule;
	}

	++(mapRules[_Rule.Value.KeyValue].RuleCount);
}

void RuleManager::RemoveRule(const Rule& _Rule)
{
	std::map<int, Rule>::iterator FindIter = mapRules.find(_Rule.Value.KeyValue);
	std::map<int, Rule>::iterator EndIter = mapRules.end();

	if (FindIter == EndIter)
	{
		MsgAssert("�������� �ʴ� Rule�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	--(FindIter->second.RuleCount);

	if (0 >= FindIter->second.RuleCount)
	{
		mapRules.erase(FindIter);
		return;
	}
}

void RuleManager::RuleReset()
{
	mapRules.clear();
}