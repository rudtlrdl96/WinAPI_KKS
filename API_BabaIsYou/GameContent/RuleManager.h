#pragma once
#include <map>

class Rule;
class RuleManager
{
	friend Rule;
public:
	static RuleManager* GetInst()
	{
		static RuleManager Instance;
		return &Instance;
	}

protected:

private:
	RuleManager();
	~RuleManager();

	RuleManager(const RuleManager& _Other) = delete;
	RuleManager(RuleManager&& _Other) noexcept = delete;
	RuleManager& operator=(const RuleManager& _Other) = delete;
	RuleManager& operator=(RuleManager&& _Other) noexcept = delete;

	void AddRule(Rule* _Rule);
	void RemoveRule(Rule* _Rule);

	std::map<int, Rule*> mapRules;
};