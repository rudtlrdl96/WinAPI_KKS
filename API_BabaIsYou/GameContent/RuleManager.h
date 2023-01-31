#pragma once
#include <map>
#include "ContentEnum.h"

class Rule;
class PuzzleActor;
class RuleManager
{
	friend Rule;
public:
	static RuleManager* GetInst()
	{
		static RuleManager Instance;
		return &Instance;
	}

	void AddRule(Rule* _Rule);
	void RemoveRule(Rule* _Rule);
	void AddActorRule(PuzzleActor* _Actor, int _ActorIndex);

protected:

private:
	RuleManager();
	~RuleManager();

	RuleManager(const RuleManager& _Other) = delete;
	RuleManager(RuleManager&& _Other) noexcept = delete;
	RuleManager& operator=(const RuleManager& _Other) = delete;
	RuleManager& operator=(RuleManager&& _Other) noexcept = delete;

	std::map<int, Rule*> mapAllRules;
};