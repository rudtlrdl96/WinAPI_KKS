#include "RuleManager.h"
#include "Rule.h"

RuleManager::RuleManager()
{
}

RuleManager::~RuleManager()
{
	std::map<int, Rule*>::iterator LoopIter = mapRules.begin();
	std::map<int, Rule*>::iterator EndIter = mapRules.end();

	for ( ;LoopIter != EndIter; ++LoopIter)
	{
		delete LoopIter->second;
		LoopIter->second = nullptr;
	}

	mapRules.clear();
}

void AddRule(Rule* _Rule)
{

}

void RemoveRule(Rule* _Rule)
{
	
}