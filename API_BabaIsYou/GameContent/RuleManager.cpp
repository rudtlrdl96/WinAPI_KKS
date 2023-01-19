#include "RuleManager.h"
#include <GameEngineBase/GameEngineDebug.h>

#include "ContentEnum.h"
#include "GridActor.h"

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

		switch (static_cast<RULE_TYPE>(_Rule.Value.DefineValue))
		{
		case RULE_TYPE::SUBJECT:
			break;
		case RULE_TYPE::DEFINE:
			break;
		default:
			break;
		}
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